#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <opencv\highgui.h>

#define forest_cols 132
#define forest_rows 63

using namespace cv;
using namespace std;

void create_sample(Mat image, Mat arr[forest_cols][forest_rows], const int cols, const int rows) {
	int rshift = 0, dshift = 0;

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			Rect r(0 + rshift, 0 + dshift, 20, 20);
			Mat copy = image;
			Mat crop = copy(r);
			arr[i][j] = crop;
			dshift += 10;
			if (j == rows - 1) dshift = 0;
		}
		rshift += 10;
	}
}

int main() {
	Mat img = imread("G:\\STUDY\\ДИПЛОМ\\examples\\1.jpg", 0);
	Mat forest[forest_cols][forest_rows];
	create_sample(img, forest, forest_cols, forest_rows);
		
	

	namedWindow("Display window", CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", forest[0][0]);
	waitKey(0);
}
