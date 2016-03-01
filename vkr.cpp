#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <opencv\highgui.h>

using namespace cv;
using namespace std;

int main() {
	Mat img = imread("G:\\STUDY\\ДИПЛОМ\\examples\\1.jpg", 0);

	int rshift = 0, dshift = 0;
	//const int str = 63, stolb = 132;
	const int str = 3, stolb = 5;
	Mat sample[stolb][str];
	for (int i = 0; i < stolb; i++) {
		for (int j = 0; j < str; j++) {
			Rect r(0 + rshift, 0 + dshift, 20, 20); //выделяем нужный фрагмент
			Mat copy = img;
			Mat crop = copy(r);
			sample[i][j] = crop; //записываем фрагмент в массив
			dshift += 10;
			if (j == str) dshift = 0; 
		}
		rshift += 10;
	}

	namedWindow("Display window", CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", sample[0][0]);
	waitKey(0);
}