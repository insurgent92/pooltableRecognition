#pragma once
#include <iostream>
#include "Histogram1D.hpp"
#include "use_opencv.h"


Histogram1D::Histogram1D() { // 1차원 히스토그램을 위한 인자 준비
	histSize[0] = 256;

	hranges[0] = 0.0;
	hranges[1] = 255.0;
	ranges[0] = hranges;

	channels[0] = 0; // 기본적으로 채널을 0으로 보기
}

// 정의한 멤버 변수로 그레이레벨 영상의 히스토그램을 계산할 때는 다음 메소드를 사용해 수행
std::vector<cv::MatND> Histogram1D::getHistogram(const cv::Mat &image) {
	// 1차원(1D) 히스토그램 계산.
	cv::Mat HSVImg;
	cvtColor(image, HSVImg, CV_BGR2HSV_FULL);

	std::vector<cv::Mat> HSV_planes;
	std::vector<cv::MatND> hists(3);

	cv::split(HSVImg, HSV_planes);

	cv::calcHist(&HSV_planes[0], // 히스토그램 계산 
		1,   // 단일 영상의 히스토그램만
		channels, // 대상 채널               
		cv::Mat(), // 마스크 사용하지 않음     
		hists[0],  // 결과 히스토그램         
		1,   // 1차원(1D) 히스토그램           
		histSize, // 빈도수                  
		ranges  // 화소값 범위             
	);

	cv::calcHist(&HSV_planes[1], // 히스토그램 계산 
		1,   // 단일 영상의 히스토그램만
		channels, // 대상 채널               
		cv::Mat(), // 마스크 사용하지 않음     
		hists[1],  // 결과 히스토그램         
		1,   // 1차원(1D) 히스토그램           
		histSize, // 빈도수                  
		ranges  // 화소값 범위             
	);

	cv::calcHist(&HSV_planes[2], // 히스토그램 계산 
		1,   // 단일 영상의 히스토그램만
		channels, // 대상 채널               
		cv::Mat(), // 마스크 사용하지 않음     
		hists[2],  // 결과 히스토그램         
		1,   // 1차원(1D) 히스토그램           
		histSize, // 빈도수                  
		ranges  // 화소값 범위             
	);
	return hists;
}
