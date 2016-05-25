#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include "opencv2/opencv.hpp"
#include <time.h>

using namespace std;
using namespace cv;
using namespace ml;

#define side 15
//1334 645
//132 63 - 10
//88 43 - 15
//66 32 - 20
//53 25 - 25
#define my_cols 88	
#define my_rows 43

clock_t start_, end_;

void create_sample(Mat image, Mat **arr) {
	int rshift = 0, dshift = 0;

	for (int i = 0; i < my_cols; i++) {
		for (int j = 0; j < my_rows; j++) {
			Rect r(0 + rshift, 0 + dshift, side, side);
			Mat copy = image;
			Mat crop = copy(r);
			arr[i][j] = crop;
			dshift += side;
			if (j == my_rows - 1) dshift = 0;
		}
		rshift += side;
	}
}

int main() {
	int num_files = 2 * my_cols * my_rows;
	int img_area = side * side;
	Mat training_mat(num_files, img_area, CV_8UC1);
	
	Mat positive = imread("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\sample_forest.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat negative = imread("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\forest_negative.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	Mat **arr_positive_sample = new Mat *[my_cols];
	Mat **arr_negative_sample = new Mat *[my_cols];
	for (int i = 0; i < my_cols; i++) {
		arr_positive_sample[i] = new Mat[my_rows];
		arr_negative_sample[i] = new Mat[my_rows];
	}
	
	create_sample(positive, arr_positive_sample);
	create_sample(negative, arr_negative_sample);

	int file_num = 0, column = 0;
	for (int k = 0; k < my_cols; k++) {
		for (int l = 0; l < my_rows; l++) {
			for (int i = 0; i < side; i++) {
				for (int j = 0; j < side; j++) {
					training_mat.at<uchar>(file_num, column++) = arr_positive_sample[k][l].at<uchar>(i, j);
				}
			}
			file_num++;
			column = 0;
		}
	}
	for (int i = 0; i < my_cols; i++)
		delete[]arr_positive_sample[i];
	delete[]arr_positive_sample;

	int border = file_num;
	for (int k = 0; k < my_cols; k++) {
		for (int l = 0; l < my_rows; l++) {
			for (int i = 0; i < side; i++) {
				for (int j = 0; j < side; j++) {
					training_mat.at<uchar>(file_num, column++) = arr_negative_sample[k][l].at<uchar>(i, j);
				}
			}
			file_num++;
			column = 0;
		}
	}
	for (int i = 0; i < my_cols; i++)
		delete[]arr_negative_sample[i];
	delete[]arr_negative_sample;

	// Data for visual representation
	//Mat image = Mat::zeros(645, 1334, CV_8UC3);
	Mat image = imread("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image_c = imread("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\4.jpg", CV_LOAD_IMAGE_COLOR);
	// Set up training data
	Mat trainingDataMat = training_mat;
	trainingDataMat.convertTo(trainingDataMat, CV_32FC1);
	Mat labelsMat(num_files, 1, CV_32SC1);
	for (int i = 0; i < num_files; i++) {
		if (i < border) labelsMat.at<int>(i, 0) = 1;
		else labelsMat.at<int>(i, 0) = -1;
	}
	// Train the SVM
	cout << "Starting training process" << endl;
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setC(0.1);
	svm->setKernel(SVM::RBF);
	svm->setGamma(0.003);
	svm->setDegree(0.5);
	svm->setCoef0(1);
	svm->setNu(0.5);
	svm->setP(0);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-4));
	svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
	//svm->save("SVM.xml");
	/*Ptr<TrainData> tr_auto = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	svm->trainAuto(tr_auto, 10, SVM::getDefaultGrid(SVM::C), SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
		SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF), SVM::getDefaultGrid(SVM::COEF), false);*/
	cout << "Finished training process" << endl;
	
	// Show the decision regions given by the SVM
	
	//Ptr<SVM> svm = Algorithm::load<SVM>("SVM.xml");
	start_ = clock();
	int rshift = 0, dshift = 0;
	for (int i = 0; i < my_cols; i++) {
		for (int j = 0; j < my_rows; j++) {
			Rect r(0 + rshift, 0 + dshift, side, side);
			Mat crop = image(r);
			Mat copy(1, img_area, CV_32F);
			int column = 0;
			for (int i = 0; i < side; i++) {
				for (int j = 0; j < side; j++) {
					copy.at<float>(0, column++) = crop.at<uchar>(i, j);
				}
			}
			float response = svm->predict(copy);
			if (response == 1) image_c(r) += CV_RGB(0, 0, 255);
			//else if (response == -1) image_c(r) += CV_RGB(200, 0, 0);
			dshift += side;
			if (j == my_rows - 1) dshift = 0;
		}
		rshift += side;
	}
	end_ = clock();
	cout << ((double)end_ - start_) / ((double)CLOCKS_PER_SEC) << endl;
	//imwrite("G:\\STUDY\\ÄÈÏËÎÌ\\examples\\ya.jpg", image);        // save the image
	imshow("SVM Simple Example", image_c); // show it to the user
	waitKey(0);
	
	system("pause");
	return 0;
}
