#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv\highgui.h>
#include <time.h>   
#include <string>
#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/ml.hpp"
#include "math.h"
//---------------------------------------------------------------------------------------------------------------------------------------//
#define side 15
//1334 645
//132 63 - 10
//88 43 - 15
//66 32 - 20
//53 25 - 25
#define cols 3
#define rows 3
bool flag = 0;
clock_t start_, end_;
//---------------------------------------------------------------------------------------------------------------------------------------//
using namespace cv;
using namespace std;
//---------------------------------------------------------------------------------------------------------------------------------------//
void create_sample(Mat image, Mat arr[cols][rows]) {
	int rshift = 0, dshift = 0;
	
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			Rect r(0 + rshift, 0 + dshift, side, side);
			Mat copy = image;
			Mat crop = copy(r);
			arr[i][j] = crop;
			dshift += side;
			if (j == rows - 1) dshift = 0;
		}
		rshift += side;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------//
bool hist_comparison(Mat etalon, Mat test_crop)
{
	Mat hsv_etalon, hsv_test_crop;
	cvtColor(etalon, hsv_etalon, COLOR_BGR2HSV);
	cvtColor(test_crop, hsv_test_crop, COLOR_BGR2HSV);

	int h_bins = 10; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	/// Histograms
	Mat hist_etalon;
	Mat hist_test_crop;

	/// Calculate the histograms for the HSV images
	calcHist(&hsv_etalon, 1, channels, Mat(), hist_etalon, 2, histSize, ranges, true, false);
	normalize(hist_etalon, hist_etalon, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_test_crop, 1, channels, Mat(), hist_test_crop, 2, histSize, ranges, true, false);
	normalize(hist_test_crop, hist_test_crop, 0, 1, NORM_MINMAX, -1, Mat());

	/// Apply the histogram comparison methods
	/*
	for (int i = 0; i < 4; i++)
	{
		int compare_method = i;
		double test = compareHist(hist_etalon, hist_test_crop, compare_method);

		printf(" Method [%d]: %f \n", i, test);
	}*/
	
	double test = compareHist(hist_etalon, hist_test_crop, CV_COMP_CORREL);
	test = abs(test);
	double match_percent = 0.5;

	if (test >= match_percent) return 1;
	else return 0;
	//return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------//
void check_and_draw(Mat arr[cols][rows], Mat test, const int c_cols, const int c_rows) {
	int rshift = 0, dshift = 0;
	start_ = clock();
	for (int i = 0; i < c_cols; i++) {
		for (int j = 0; j < c_rows; j++) {
			flag = 0;
			Rect r(0 + rshift, 0 + dshift, side, side);
			Mat copy = test;
			Mat crop = copy(r);
			for (int j = 0; j < cols; j++) {
				for (int k = 0; k < rows; k++) {
					if (hist_comparison(arr[j][k], crop)) {
						test(r) += CV_RGB(0, 0, 255);
						flag = 1;
					}
					if (flag == 1) break;
				}
				if (flag == 1) break;
			}
			dshift += side;
			if (j == c_rows - 1) dshift = 0;
		}
		rshift += side;
	}

	end_ = clock();
	cout << ((double)end_ - start_) / ((double)CLOCKS_PER_SEC) << endl;

	namedWindow("Demo", CV_WINDOW_AUTOSIZE);
	imshow("Demo", test);
	waitKey(0);
}
//---------------------------------------------------------------------------------------------------------------------------------------//
int main() {
	Mat img_forest = imread("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\shw.jpg", CV_LOAD_IMAGE_COLOR);
	Mat img_forest_test = imread("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\4.jpg", CV_LOAD_IMAGE_COLOR);
	
	/*Rect r(0, 0, 20, 20);
	Mat test_crop = img_forest(r);*/
	
	Mat forest[cols][rows];
	create_sample(img_forest, forest);

	check_and_draw(forest, img_forest_test, 88, 43);
	
	system("pause");
	return 0;
}
