#include "image.h"
#include "log.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <iostream>
using namespace std;

/* Parameter for Freese Image Detection */
double freese_threshold = 3;
/* Parameter for Black Image Detection */
int black_threshold = 30;
/* Parameters For Mosaic Detection */
int BlockSize = 16;
int canny_threshold = 150;
int T_edge    = 14;
int T_avg_gray = 100;
int T_sdv_gray = 10;
int T_edge_size = 30;
int T_mosaic_count = 3;


bool is_freezing(const IplImage *pImg1, const IplImage *pImg2)
{
	IplImage * pImgTmp = NULL;

	pImgTmp = cvCloneImage(pImg1);
	//cvSub(pImg1, pImg2, pImgTmp);
	cvAbsDiff(pImg1, pImg2, pImgTmp);

	CvScalar avg, sdv;
	cvAvgSdv(pImgTmp, &avg, &sdv);	
	cvReleaseImage(&pImgTmp);
	return ((avg.val[0]+sdv.val[0]) < (freese_threshold));
}

bool is_freezing2(const IplImage *pImg1, const IplImage *pImg2)
{
	bool bEqual = false;

	CvHistogram *pHist1 = NULL;
	CvHistogram *pHist2 = NULL;

	// Initialize Histograms
	int dims = 1;// dims of histogram
	int sizes[1] = {255};
	int type = CV_HIST_ARRAY;
	float ** ranges = NULL;
	int uniform = 1;
	pHist1 = cvCreateHist(dims, sizes, type, ranges, uniform);
	pHist2 = cvCreateHist(dims, sizes, type, ranges, uniform);    

	double factor = 10000;

	/// calc color histograms 
	IplImage *planes[1] = {const_cast<IplImage*>(pImg1)};
	cvCalcHist(planes, pHist1);
	planes[0] = const_cast<IplImage*>(pImg2);
	cvCalcHist(planes, pHist2);
	bool bNormalize = true;

	if (bNormalize)
	{
		cvNormalizeHist(pHist1, factor);
		cvNormalizeHist(pHist2, factor);
	}

	// compare 2 images 
	//int method = CV_COMP_CHISQR;
	double comp = 0;
	comp = cvCompareHist(pHist1, pHist2, CV_COMP_CORREL);
	cout << "comp = " << comp << endl;
	if (comp < freese_threshold)
	{
		bEqual = true;
	}

	cvReleaseHist(&pHist1);
	cvReleaseHist(&pHist2);
	return bEqual;
}

bool is_black(const IplImage *pImg)
{
	CvScalar mean;
	CvScalar std_dev;
	cvAvgSdv(pImg, &mean, &std_dev);
	int nmean = (int)mean.val[0];
	int nstd_dev = (int)std_dev.val[0];

	return (nmean < 5 && nstd_dev < black_threshold);
}

bool is_pure(const IplImage *pImg)
{
	CvScalar mean;
	CvScalar std_dev;
	cvAvgSdv(pImg, &mean, &std_dev);
	//int nmean = (int)mean.val[0];
	int nstd_dev = (int)std_dev.val[0];

	return (nstd_dev < black_threshold);
}



enum EnumDirection
{
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3    
};



struct MacroBlock
{
	int avg_gray;
	int sdv_gray;
	int edge_size;
	bool edge[4];
	bool mosaic; 
	int edgeType()
	{
		int magic = edge[TOP]*0x1000 + edge[RIGHT]*0x0100 + 
			edge[BOTTOM]*0x0010 + edge[LEFT];
		switch(magic)
		{
			case 0x0000:
				return 0;	    
			case 0x1000:
				return 1;
			case 0x0010:
				return 2;
			case 0x1010:
				return 3;
			case 0x1011:
				return 4;
			case 0x1110:
				return 5;
			case 0x1111:
				return 6;
			case 0x1001:
				return 7;
			default:
				return 8;
		}	    
	}    

};




bool is_mosaic(const IplImage *pGray)
{
	int edge_size_filtered = 0;
	//int avg_gray_filtered = 0;
	//int sdv_gray_filtered = 0;

	IplImage *pEdge = NULL;
	
	CvSize size = cvGetSize(pGray);

	int channels = 1;

	pEdge = cvCreateImage(size, IPL_DEPTH_8U, channels);
	cvEqualizeHist(pGray, pEdge);

	/* edge detection */
	int aperture_size = 3;
	cvCanny(pEdge, pEdge, canny_threshold, canny_threshold * 0.4, aperture_size);   

	/* mosaic detection */
	int h = size.height / BlockSize;
	int w = size.width / BlockSize;
	struct MacroBlock **macros = NULL; 

	macros = new MacroBlock *[w];
	for (int i = 0; i < w; i++)
	{
		macros[i] = new MacroBlock[h];
	}


	IplImage *pColor = cvCreateImage(size, IPL_DEPTH_8U, 3);
	cvCvtColor(pEdge, pColor, CV_GRAY2BGR);


	CvPoint p1, p2;
	//int LineWidth = 0;
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			// 第(x, y)个宏块
			int top = y * BlockSize ;
			int left = x * BlockSize ;
			int bottom = y * BlockSize + BlockSize - 1;
			int right = x * BlockSize + BlockSize - 1;

			int count;
			// top
			count = 0;
			for (int i = left; i < right; i++)
			{
				int data = 0;
				data += (top == 0) ? 0 : (bool)cvGetReal2D(pEdge, top-1, i); 
				data += (bool)cvGetReal2D(pEdge, top, i);
				data += (bool)cvGetReal2D(pEdge, top+1, i);
				//data += (bool)cvGetReal2D(pEdge, top+2, i);
				if (data)
					count++;
			}
			macros[x][y].edge[TOP] = (count >= T_edge);

			// right
			count = 0;
			for (int i = top; i < bottom; i++)
			{
				int data = 0;
				data += (bool)cvGetReal2D(pEdge, i, right-1); 
				data += (bool)cvGetReal2D(pEdge, i, right);
				data += (right+1 >= size.width) ? 0 : (bool)cvGetReal2D(pEdge, i, right+1);
				//data += (right+2 >= size.width) ? 0 : (bool)cvGetReal2D(pEdge, i, right+2);
				if (data)
					count++;
			}
			macros[x][y].edge[RIGHT] = (count >= T_edge);

			// bottom
			count = 0;
			for (int i = left; i < right; i++)
			{
				int data = 0;
				data += (bool)cvGetReal2D(pEdge, bottom-1, i); 
				data += (bool)cvGetReal2D(pEdge, bottom, i);
				data += (bottom+1 >= size.height) ? 0 : (bool)cvGetReal2D(pEdge, bottom+1, i);
				//data += (bottom+2 >= size.height) ? 0 : (bool)cvGetReal2D(pEdge, bottom+2, i);
				if (data)
					count++;
			}	    
			macros[x][y].edge[BOTTOM] = (count >= T_edge);

			// left
			count = 0;
			for (int i = top; i < bottom; i++)
			{
				int data = 0;
				data += (left == 0) ? 0 : (bool)cvGetReal2D(pEdge, i, left-1); 
				data += (bool)cvGetReal2D(pEdge, i, left);
				data += (bool)cvGetReal2D(pEdge, i, left+1);
				data += (bool)cvGetReal2D(pEdge, i, left+2);
				if (data)
					count++;
			}
			macros[x][y].edge[LEFT] = (count >= T_edge);


			if (macros[x][y].edge[TOP])
			{
				p1 = cvPoint(left, top);
				p2 = cvPoint(right, top);
				cvLine(pColor, p1, p2, CV_RGB(255, 0, 0), 1, CV_AA, 0);
			}
			if (macros[x][y].edge[RIGHT])
			{
				p1 = cvPoint(right, top);
				p2 = cvPoint(right, bottom);
				cvLine(pColor, p1, p2, CV_RGB(255, 0, 0), 1, CV_AA, 0);
			}
			if (macros[x][y].edge[BOTTOM])
			{
				p1 = cvPoint(left, bottom);
				p2 = cvPoint(right, bottom);
				cvLine(pColor, p1, p2, CV_RGB(255, 0, 0), 1, CV_AA, 0);
			}
			if (macros[x][y].edge[LEFT])
			{
				p1 = cvPoint(left, top);
				p2 = cvPoint(left, bottom);
				cvLine(pColor, p1, p2, CV_RGB(255, 0, 0), 1, CV_AA, 0);
			}

			// 计算平均灰度值 Y_average
			CvMat submat;
			cvGetSubRect(pGray, &submat, cvRect(left, top, BlockSize, BlockSize));
			CvScalar mean, sdv;
			cvAvgSdv(&submat, &mean, &sdv);
			macros[x][y].avg_gray = (int)mean.val[0];
			macros[x][y].sdv_gray = (int)sdv.val[0];

			// 计算宏块内的边缘点个数
			cvGetSubRect(pEdge, &submat, cvRect(left, top, BlockSize, BlockSize));	    
			macros[x][y].edge_size = cvCountNonZero(&submat);
		} 
	}


	//cvShowImage("mosaic", pColor);
	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{

			if (i % BlockSize == 0 ||
					j % BlockSize == 0)
				cvSetReal2D(pEdge, i, j, 200);
		}
	}


	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			MacroBlock & mb = macros[i][j];
			int edgeType = mb.edgeType();

			if (edgeType > 2 && edgeType < 7)
			{
				mb.mosaic = true;
			}
			else if (edgeType == 7)
			{		
				for (int k = 1; k <= 3; k++)
				{
					MacroBlock & tmpMb = macros[i][j+k];
					if (tmpMb.avg_gray > T_avg_gray)
						break;
					if (tmpMb.edgeType() == 2)
					{
						for (int l = 0; l <= k ; l++)
							macros[i][j+l].mosaic = true;
						break;
					}
				}
			}
			else 
			{
				mb.mosaic = false;		
			}


			if (mb.mosaic){
				if (mb.edge_size > T_edge_size)
				{
					mb.mosaic = false;
					edge_size_filtered ++;
				}/*else if (mb.avg_gray > T_avg_gray)
				{
					mb.mosaic = false;
					avg_gray_filtered ++;
				}
				else if (mb.sdv_gray > T_sdv_gray){
					mb.mosaic = false;
					sdv_gray_filtered ++;
				}*/
			}
		}
	}

	int mosaic_count = 0;
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{	  
			if (macros[i][j].mosaic)
			{
				mosaic_count ++;
			}
		}
	}


	for (int i = 0; i < w; i++)
	{
		delete[] macros[i];
	}
	delete[] macros;
	macros = NULL;

	/* release images */
	cvReleaseImage(&pEdge);
	cvReleaseImage(&pColor);
	if (mosaic_count)
		cout << "mosaic_count = " << mosaic_count << endl;

	return (mosaic_count >= T_mosaic_count);
}
