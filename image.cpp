#include "image.h"
#include "log.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <iostream>
using namespace std;



/** 配置几种检测算法的阀值 **/
struct config {
	/** 静帧检测的阀值 
	 *  默认值 3
	 **/
	int freese_th;

	/** 黑屏检测的阀值
	 *  默认值 30
	 **/
	int black_th;

	/** 
	 * 马赛克检测的3个阀值
	 */
	 /* 判断宏块边的是否"实边"的阀值
	  * 当宏块边的二阶梯度>该值时,它是"实边"
	  * 默认值 8, 
	  * 建议 5-10吧
	  */
	int mosaic_point_th;
	 /* 判断宏块是"马赛克"的阀值
	  * 如果宏块的实边个数>该值,它是"马赛克"
	  * 默认值 2, 
	  * 建议 2或3 吧
	  */
	int mosaic_line_th;
	/** 判断是否马赛克图像的阀值
	 *  图像中马赛克的个数大于该阀值即为马赛克图像
	 *  默认值 5
	 *  建议 4-10吧
	 */
	int mosaic_count_th;  	

	/** 马赛克图像处理时是否影响邻居宏块
	 * 如果mosaic_line_th<4时,如果本参数为1,
	 * 将已经判断为马赛克的宏块的非实边全部置为实边
	 * 同时重新处理邻居宏块
	 * 默认值 1
	 */
	int mosaic_adjacent_effect;
	config();

	void init(int freese_th, int black_th, 
			 int mosaic_point_th, int mosaic_line_th, int mosaic_count_th,
			 int mosaic_adjacent_effect);
};

config::config(){
	init(3, 30, 8, 3, 5, 1);
}

void config::init(int freese_th = 3, int black_th = 30, 
		 int mosaic_point_th = 8, int mosaic_line_th = 3, int mosaic_count_th = 5, 
		 int mosaic_adjacent_effect = 1)
{
	this->freese_th = freese_th;
	this->black_th = black_th;
	this->mosaic_count_th = mosaic_count_th;
	this->mosaic_point_th = mosaic_point_th;
	this->mosaic_line_th = mosaic_line_th;
}

struct config g_config;



/*************************************************************************************
 *                               Freezing Detection 
 *************************************************************************************/
bool is_freezing(const IplImage *pImg1, const IplImage *pImg2)
{
	IplImage * pImgTmp = NULL;

	pImgTmp = cvCloneImage(pImg1);
	//cvSub(pImg1, pImg2, pImgTmp);
	cvAbsDiff(pImg1, pImg2, pImgTmp);

	CvScalar avg, sdv;
	cvAvgSdv(pImgTmp, &avg, &sdv);	
	cvReleaseImage(&pImgTmp);
	return ((avg.val[0]+sdv.val[0]) < (g_config.freese_th));
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
	if (comp < g_config.freese_th)
	{
		bEqual = true;
	}

	cvReleaseHist(&pHist1);
	cvReleaseHist(&pHist2);
	return bEqual;
}



/*************************************************************************************
 *                               Black Image Detection 
 *************************************************************************************/

bool is_black(const IplImage *pImg)
{
	CvScalar mean;
	CvScalar std_dev;
	cvAvgSdv(pImg, &mean, &std_dev);
	int nmean = (int)mean.val[0];
	int nstd_dev = (int)std_dev.val[0];

	return (nmean < 5 && nstd_dev < g_config.black_th);
}

bool is_pure(const IplImage *pImg)
{
	CvScalar mean;
	CvScalar std_dev;
	cvAvgSdv(pImg, &mean, &std_dev);
	//int nmean = (int)mean.val[0];
	int nstd_dev = (int)std_dev.val[0];

	return (nstd_dev < g_config.black_th);
}




/*************************************************************************************
 *                               Mosaic Image Detection 
 *************************************************************************************/
#define BLOCK_SIZE 16
#define TOP   0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

/** 宏块数据保存 **/
/** 当前保存的是4条边的2阶梯度 **/
struct macroblock {
	int val[4];
	int bMosaic;
};

bool is_mosaic(const IplImage *pImage)
{
	if (pImage == NULL)
		return false;

	int width  = pImage->width;   // 图像宽
	int height = pImage->height;  // 图像高
	int w16    = width/16;        // x-轴上宏块个数
	int h16    = height/16;       //  y-轴上宏块个数

	struct macroblock * mb = new macroblock[w16*h16];  // 全部w16*h16个宏块
	memset(mb, 0, sizeof(macroblock)*w16*h16);
	// 计算图像的x-梯度和y-梯度值(1阶, 2阶)
	// fx'(x,y) = abs(f(x+1, y) - f(x, y))
	// fx"(x,y) = min{fx'(x,y)-f'(x-1,y), fx'(x,y)-f'(x+1,y)}
	//          = min{abs(f(x+1,y) - f(x,y)) - abs(f(x,y) - f(x-1,y)),
	//                abs(f(x+1,y) - f(x,y)) - abs(f(x+2,y) - f(x+1,y))}
	//              
	// fy'(x,y) = abs(f(x, y+1) - f(x, y))
	// fy"(x,y) = min(fy'(x, y)-fy'(x, y-1), fy'(x,y)-fy'(x,y-1)
	//          = min{abs(f(x,y+1) - f(x,y)) - abs(f(x,y) - f(x,y-1)),
	//                abs(f(x,y+1) - f(x,y)) - abs(f(x,y+2) - f(x,y+1))}
	for (int i = 0; i < h16; i++){
		for (int j = 0; j < w16; j++){
			// 从h16*w16, 遍历每个宏块,计算其顶边和左边的2阶梯度,
			// 注意,每个宏块的top边也是顶上一个宏块的bottom边,
			//    其left边是它左边的宏块的right边
			// 因此每个宏块只需要计算２条边

			int top = i ? (i*16-1) : 0;
			int left = j ? (j*16-1) : 0;
					
			// fy'(x,y) = abs(f(x, y+1) - f(x, y))
			// fy"(x,y) = min(fy'(x, y)-fy'(x, y-1), fy'(x,y)-fy'(x,y-1)
			//          = min{abs(f(x,y+1) - f(x,y)) - abs(f(x,y) - f(x,y-1)),
			//                abs(f(x,y+1) - f(x,y)) - abs(f(x,y+2) - f(x,y+1))}
			int ddy;
			for (int k = 0; i > 0 && k < 16; k++){
				// 按一条宏块边16个象素计算
				int y[4];
				y[0] = cvGetReal2D(pImage, top-1, left+k);	
				y[1] = cvGetReal2D(pImage, top, left+k);
				y[2] = cvGetReal2D(pImage, top+1, left+k);
				y[3] = cvGetReal2D(pImage, top+2, left+k);
				
				// 一阶y梯度
				int dy[3];
				dy[0] = abs(y[1] - y[0]);
				dy[1] = abs(y[2] - y[1]);  
				dy[2] = abs(y[3] - y[2]);
				
				// 二阶y梯度
				int tmp = min(dy[1] - dy[0], dy[1] - dy[2]);
				ddy += tmp;
			}
			ddy = (ddy/16); // 边上平均每个点的二阶梯度

			mb[i*w16+j].val[TOP] = ddy; 
			if (i > 0){ 
				mb[(i-1)*w16+j].val[BOTTOM] = ddy;
			}

			// fx'(x,y) = abs(f(x+1, y) - f(x, y))
			// fx"(x,y) = min{fx'(x,y)-f'(x-1,y), fx'(x,y)-f'(x+1,y)}
			//          = min{abs(f(x+1,y) - f(x,y)) - abs(f(x,y) - f(x-1,y)),
			//                abs(f(x+1,y) - f(x,y)) - abs(f(x+2,y) - f(x+1,y))}
			int ddx = 0;
			for (int k = 0; j > 0 && k < 16; k++){
				// 按一条宏块边16个象素计算
				int x[4];
				x[0] = cvGetReal2D(pImage, top+k, left-1);	
				x[1] = cvGetReal2D(pImage, top+k, left);
				x[2] = cvGetReal2D(pImage, top+k, left+1);
				x[3] = cvGetReal2D(pImage, top+k, left+2);

				// 一阶x梯度
				int dx[3];
				dx[0] = abs(x[1] - x[0]);
				dx[1] = abs(x[2] - x[1]);
				dx[2] = abs(x[3] - x[2]);
				
				// 二阶x梯度
				int tmp = min(dx[1] - dx[0], dx[1] - dx[2]);
				ddx += tmp;
			}

			ddx = (ddx/16); // 边上平均每个点的二阶梯度
			mb[i*w16+j].val[LEFT] = ddx;
			if (j > 0){
				mb[i*w16+j-1].val[RIGHT] = ddx;
			}
		}
	}

#ifdef WITHOUT_SDK
	IplImage *pMosaic = cvCreateImage(cvSize(width, height), 8, 3);
	IplImage *p2D = cvCreateImage(cvSize(width, height), 8, 3);
	cvCvtColor(pImage, pMosaic, CV_GRAY2BGR);
#endif

	int mosaic_num = 0;
	int dirty = 0;
	int loop = 0;
	do {
		dirty = 0;
		for (int i = 0; i < h16; i++){
			for (int j = 0; j < w16; j++){
				// 遍历每个宏块
				// 如果宏块的边的二阶梯度值>mosaic_point_th
				// 判断该边为"实边"
				int n_edges = 0;
				struct macroblock *pmb = &mb[i*w16+j];
				if (pmb->bMosaic) 
					continue;
				
				bool bEdge[4] = {0};
				for (int k = 0; k < 4; k++){
					bEdge[k] = (pmb->val[k] >= g_config.mosaic_point_th);
					if (bEdge[k]){
						n_edges ++;
					}
				}	
				
#ifdef WITHOUT_SDK
				int top = i*16;
				int left = j*16;
				int ddy = mb[i*w16+j].val[TOP];
				int ddx = mb[i*w16+j].val[LEFT];
				cvLine(p2D, cvPoint(left, top), cvPoint(left+15, top), 
						CV_RGB(0, ddy*10, 0), 1, CV_AA, 0);
				cvLine(p2D, cvPoint(left, top), cvPoint(left, top+15), 
						CV_RGB(0, ddx*10, 0), 1, CV_AA, 0);
				cvSet2D(pMosaic, top, left, CV_RGB(255, 0, 0));
#endif

			
				// 如果宏块的"实边"个数>mosaic_line_th
				// 判断该宏块为"马赛克" 
				if (n_edges >= g_config.mosaic_line_th){
					mosaic_num ++;	
					if (g_config.mosaic_adjacent_effect && n_edges < 4){
						// 马赛克宏块的4条边都设置成实边,漫延到邻居的宏块
						if (i > 0 && !bEdge[TOP]){
							mb[(i-1)*w16+j].val[BOTTOM] = 255;
							dirty = 1;
						}
						if (j > 0 && !bEdge[LEFT]){
							mb[i*w16+j-1].val[RIGHT] = 255;
							dirty = 1;
						}
						if (j < h16-1 && !bEdge[BOTTOM]){
							mb[(i+1)*w16+j].val[TOP] = 255;
							dirty = 1;
						}
						if (j < w16-1 && !bEdge[RIGHT]){
							mb[i*w16+j+1].val[LEFT] = 255;
							dirty = 1;
						}
					}
#ifdef WITHOUT_SDK
					cvRectangle(pMosaic, cvPoint(left, top), cvPoint(left+15, top+15), 
							CV_RGB(255, 0, 0), 1, CV_AA, 0);
#endif
				}
			}
		}
		loop ++;
	}while (g_config.mosaic_adjacent_effect && dirty);
	LOG("Loop %d times", loop);
	delete[] mb;

#ifdef WITHOUT_SDK
	cvShowImage("mosaic", pMosaic);
	cvShowImage("2-degree", p2D);
	cvReleaseImage(&p2D);
	cvReleaseImage(&pMosaic);
#endif
	LOG("mosaics=%d", mosaic_num);
	// 如果"马赛克"的图像个数>mosaic_count_th, 判断该图像为马赛克图像
	return (mosaic_num > g_config.mosaic_count_th);
}


void save_image(const IplImage *image, const char* prefix = ""){
	if (image == NULL)
		return ;

	char path[1024];
	sprintf(path, IMAGE_PATH "/%s_%s.jpg", prefix, time2str(time(NULL)));
	cvSaveImage(path, image);
}
