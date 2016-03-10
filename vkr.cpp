#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <opencv\highgui.h>
//---------------------------------------------------------------------------------------------------------------------------------------//
#define forest_cols 132
#define forest_rows 63

#define water_cols 132
#define water_rows 63
//---------------------------------------------------------------------------------------------------------------------------------------//
using namespace cv;
using namespace std;
//---------------------------------------------------------------------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------------------------------------------------------------------//
struct Pixels {
public:
	Pixels(short b, short g, short r, int c) {
		B = b;
		G = g;
		R = r;
		count = c;
	}
	short B;
	short G;
	short R;
	int count;
};
//---------------------------------------------------------------------------------------------------------------------------------------//
void square_stat(Mat square, vector <Pixels> &stat) {
	stat.push_back(Pixels(square.at<Vec3b>(0, 0).val[0], square.at<Vec3b>(0, 0).val[1], square.at<Vec3b>(0, 0).val[2], 0));
	bool flag;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			flag = 0;
			for (int k = 0; k < stat.size(); k++) {
				if ((square.at<Vec3b>(i, j).val[0] == stat[k].B) &&
					(square.at<Vec3b>(i, j).val[1] == stat[k].G) &&
					(square.at<Vec3b>(i, j).val[2] == stat[k].R)) {
					stat[k].count++;
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				stat.push_back(Pixels(square.at<Vec3b>(i, j).val[0], square.at<Vec3b>(i, j).val[1], square.at<Vec3b>(i, j).val[2], 1));
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------//
int main() {
	Mat img_forest = imread("G:\\STUDY\\ДИПЛОМ\\examples\\1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat forest[forest_cols][forest_rows];
	create_sample(img_forest, forest, forest_cols, forest_rows);
	
	/*
	Mat img_water = imread("G:\\STUDY\\ДИПЛОМ\\examples\\2.jpg", 0);
	Mat water[water_cols][water_rows];
	create_sample(img_water, water, water_cols, water_rows);
	*/

	vector <Pixels> forest_stat_0;
	square_stat(forest[0][0], forest_stat_0);
		
	for (int i = 0; i <  forest_stat_0.size(); i++)
		cout << i + 1 << ". B=" << forest_stat_0[i].B
		<< ", G=" << forest_stat_0[i].G
		<< ", R=" << forest_stat_0[i].R
		<< ", count=" << forest_stat_0[i].count << endl;
	system("pause");
	/*namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	imshow("Display window", forest[0][0]);
	waitKey(0);*/
	return 0;
}
