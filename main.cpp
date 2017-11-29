#include "use_opencv.h"
#include <iostream>
#include "Histogram1D.hpp"

using namespace std;
using namespace cv;

cv::Mat four_point_transform(cv::Mat input, vector<Point2f> vec){
	Mat dstMat;
	Point2f tl, tr, br, bl;
	tl = vec[0];
	tr = vec[1];
	br = vec[2];
	bl = vec[3];

	int widthA = sqrt(((br.x - bl.x)*(br.x - bl.x)) + ((br.y - bl.y) *(br.y - bl.y)));
	int widthB = sqrt(((tr.x - tl.x) *(tr.x - tl.x)) + ((tr.y - tl.y) *(tr.y - tl.y)));
	int maxWidth = max(int(widthA), int(widthB));

	int heightA = sqrt(((tr.x - br.x) * (tr.x - br.x)) + ((tr.y - br.y) *(tr.y - br.y)));
	int heightB = sqrt(((tl.x - bl.x) *(tl.x - bl.x)) + ((tl.y - bl.y) *(tl.y - bl.y)));
	int maxHeight = max(int(heightA), int(heightB));

	maxWidth = 800;
	maxHeight = 600;
	vector<Point2f> v2;

	Point2f pt;
	pt.x = 0;
	pt.y = 0;
	v2.push_back(pt);

	pt.x = maxWidth - 1;
	pt.y = 0;
	v2.push_back(pt);

	pt.x = maxWidth - 1;
	pt.y = maxHeight - 1;
	v2.push_back(pt);

	pt.x = 0;
	pt.y = maxHeight - 1;
	v2.push_back(pt);


	Mat dest2 = getPerspectiveTransform(vec, v2);
	
	warpPerspective(input, dstMat, dest2, Size(maxWidth, maxHeight));
	
	return dstMat;
}

Mat getEdgeInvImage(const Mat& src) {
	Mat gray;
	Mat retEdge;
	cvtColor(src, gray, CV_BGR2GRAY);
	Canny(gray, retEdge, 20, 150);
	retEdge = ~retEdge;
	cvtColor(retEdge, retEdge, CV_GRAY2BGR);
	return retEdge;
}

Mat getFloodFillMask(const Mat& src) {
	Mat _src;
	src.copyTo(_src);

	Scalar loDiff = Scalar::all(10);
	Scalar upDiff = Scalar::all(10);

	int floodFlag = 4 | FLOODFILL_FIXED_RANGE;
	Rect boundRect;

	cv::Mat element5(5, 5, CV_8U, cv::Scalar(255));
	cv::morphologyEx(_src, _src, cv::MORPH_ERODE, element5);


	Mat mask = Mat(Size(_src.cols + 2, _src.rows + 2), CV_8U, Scalar::all(0));

	floodFill(_src, mask, Point(300, 250), Scalar(0, 0, 255), &boundRect, loDiff, upDiff, floodFlag);

	return _src;
}

vector<Point2f> calcFourPts() {
	vector<Point2f> v1;

	Point2f pt;

	//top - left
	pt.x = 113;
	pt.y = 110;
	v1.push_back(pt);

	//top - right
	pt.x = 536;
	pt.y = 111;
	v1.push_back(pt);

	//bottom - right 
	pt.x = 536;
	pt.y = 391;
	v1.push_back(pt);

	//bottom - left
	pt.x = 110;
	pt.y = 392;
	v1.push_back(pt);

	return v1;
}

static double getGaussian(double value, double mean, double sigma) {
	return ((1.0) / (sigma * sqrt(2.0 * 3.14))) * exp((-1.0/2.0) * ((value - mean) / sigma) * (((value - mean) / sigma)));
}

int main(){

	VideoCapture cap("samples/sample1.avi", IMREAD_GRAYSCALE);
	Mat srcImage, edgeImgMat, floodfillMask;
	Histogram1D h;

	if (!cap.isOpened()){
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	for (;;){

		if (!cap.read(srcImage)){ cout << "Cannot read the frame" << endl; break;}

		resize(srcImage, srcImage, Size(640, 480));
		edgeImgMat = getEdgeInvImage(srcImage);
		floodfillMask = getFloodFillMask(edgeImgMat);
		
		vector<Point2f> v1 = calcFourPts();

		Mat warped = four_point_transform(srcImage, v1);

		

		double maxVal[3]; // 최대 빈도수 가져오기
		double minVal[3]; // 최소 빈도수 가져오기
		Point maxLoc[3] ;// 최대 빈도수 가져오기
		Point minLoc[3] ;// 최소 빈도수 가져오기

		


		Mat bgProbMap_H = Mat(warped.size(), CV_64FC1, Scalar(0.0));
		Mat bgProbMap_S = Mat(warped.size(), CV_64FC1, Scalar(0.0));
		Mat bgProbMap_V = Mat(warped.size(), CV_64FC1, Scalar(0.0));

		Mat redProbMap = Mat(warped.size(), CV_64FC1, Scalar(0.0));
		Mat yelloPobMap = Mat(warped.size(), CV_64FC1, Scalar(0.0));
		Mat whiteProbMap = Mat(warped.size(), CV_64FC1, Scalar(0.0));
		Mat thresolds = Mat(warped.size(), CV_8UC1, Scalar(0));
		std::vector<cv::MatND> hists = h.getHistogram(warped);

		cv::minMaxLoc(hists[0], &minVal[0], &maxVal[0], &minLoc[0], &maxLoc[0]);
		cv::minMaxLoc(hists[1], &minVal[1], &maxVal[1], &minLoc[1], &maxLoc[1]);
		cv::minMaxLoc(hists[2], &minVal[2], &maxVal[2], &minLoc[2], &maxLoc[2]);

		cv::Mat HSVImg;
		cvtColor(warped, HSVImg, CV_BGR2HSV_FULL);
		std::vector<cv::Mat> HSV_planes;
		cv::split(HSVImg, HSV_planes);


		for (int j = 0; j < warped.rows; j++) {
			for (int i = 0; i < warped.cols; i++) {
				double vald = getGaussian(HSV_planes[0].at<uchar>(j, i), maxLoc[0].y, 10);

				bgProbMap_H.at<double>(j, i) = vald;

				vald > 0.00001 ? thresolds.at<uchar>(j, i) = 0 : thresolds.at<uchar>(j, i) = 255;

				bgProbMap_S.at<double>(j, i) = getGaussian(HSV_planes[1].at<uchar>(j, i), maxLoc[1].y, 10);
				bgProbMap_V.at<double>(j, i) = getGaussian(HSV_planes[2].at<uchar>(j, i), maxLoc[2].y, 10);
			}
		}


		cv::Mat element5(9, 9, CV_8U, cv::Scalar(255));
		cv::morphologyEx(thresolds, thresolds, cv::MORPH_ERODE, element5);
		//cv::morphologyEx(thresolds, thresolds, cv::MORPH_ERODE, element5);
		//cv::morphologyEx(thresolds, thresolds, cv::MORPH_ERODE, element5);
		//cv::morphologyEx(thresolds, thresolds, cv::MORPH_DILATE, element5);
		//cv::morphologyEx(thresolds, thresolds, cv::MORPH_DILATE, element5);
		cv::morphologyEx(thresolds, thresolds, cv::MORPH_DILATE, element5);
		//cv::morphologyEx(thresolds, thresolds, cv::MORPH_DILATE, element5);
		Mat label,stats,centd;
		cv::connectedComponentsWithStats(thresolds, label, stats, centd);
		
		for (int i = 1; i < stats.rows; i++) {
			rectangle(warped, Rect(stats.at<int>(i, 0) - 10, stats.at<int>(i, 1) - 10, stats.at<int>(i, 2) + 20, stats.at<int>(i, 3) + 20), Scalar(0, 0, 255), 3);
		}

		//bgProbMap_H.convertTo(thresolds, CV_8UC1);
		//cv::threshold(bgProbMap_H, thresolds, 0.1, 1, THRESH_OTSU);
		circle(srcImage, Point(113, 110), 10, Scalar(0, 0, 255), -1);
		circle(srcImage, Point(536, 111), 10, Scalar(0, 0, 255), -1);
		circle(srcImage, Point(536, 391), 10, Scalar(0, 0, 255), -1);
		circle(srcImage, Point(110, 391), 10, Scalar(0, 0, 255), -1);

		imshow("thresolds", thresolds);
		imshow("MyVideo", srcImage);
		imshow("edges", edgeImgMat);
		imshow("warped", warped);
		imshow("floodfillMask", floodfillMask);

		if (waitKey(30) == 27){
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}