#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

const char err_buf[1024] = {0};

int main(int argc, char ** argv){
	if (argc != 4){
		cout << "usage: " << argv[0] << " video errsize errcount " << endl;
		return 0;
	}	

	int errsize = atoi(argv[2]);
	int errcount = atoi(argv[3]);

	if (errsize > 1024){
		cout << "max-errsize = 1024" << endl;
		return 0;
	}

	ofstream outfile(argv[1], ios_base::in | ios_base::out); 	
	if (!outfile.is_open()){
		cout << "cannot open file " << argv[1] << endl;
		return 0;
	}
	outfile.seekp(0, ios_base::end);
	long length = outfile.tellp();
	long from = length/10;
	long to   = length*9/10;
	srand(time(NULL));
	while (errcount-- > 0){
		long pos = from + rand()%(to-from);
		outfile.seekp(pos);
		outfile.write(err_buf, errsize);	
	}
		
	outfile.close();

}
