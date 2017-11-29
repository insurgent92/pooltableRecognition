#pragma once
#include <iostream>
#include "Histogram1D.hpp"
#include "use_opencv.h"


Histogram1D::Histogram1D() { // 1���� ������׷��� ���� ���� �غ�
	histSize[0] = 256;

	hranges[0] = 0.0;
	hranges[1] = 255.0;
	ranges[0] = hranges;

	channels[0] = 0; // �⺻������ ä���� 0���� ����
}

// ������ ��� ������ �׷��̷��� ������ ������׷��� ����� ���� ���� �޼ҵ带 ����� ����
std::vector<cv::MatND> Histogram1D::getHistogram(const cv::Mat &image) {
	// 1����(1D) ������׷� ���.
	cv::Mat HSVImg;
	cvtColor(image, HSVImg, CV_BGR2HSV_FULL);

	std::vector<cv::Mat> HSV_planes;
	std::vector<cv::MatND> hists(3);

	cv::split(HSVImg, HSV_planes);

	cv::calcHist(&HSV_planes[0], // ������׷� ��� 
		1,   // ���� ������ ������׷���
		channels, // ��� ä��               
		cv::Mat(), // ����ũ ������� ����     
		hists[0],  // ��� ������׷�         
		1,   // 1����(1D) ������׷�           
		histSize, // �󵵼�                  
		ranges  // ȭ�Ұ� ����             
	);

	cv::calcHist(&HSV_planes[1], // ������׷� ��� 
		1,   // ���� ������ ������׷���
		channels, // ��� ä��               
		cv::Mat(), // ����ũ ������� ����     
		hists[1],  // ��� ������׷�         
		1,   // 1����(1D) ������׷�           
		histSize, // �󵵼�                  
		ranges  // ȭ�Ұ� ����             
	);

	cv::calcHist(&HSV_planes[2], // ������׷� ��� 
		1,   // ���� ������ ������׷���
		channels, // ��� ä��               
		cv::Mat(), // ����ũ ������� ����     
		hists[2],  // ��� ������׷�         
		1,   // 1����(1D) ������׷�           
		histSize, // �󵵼�                  
		ranges  // ȭ�Ұ� ����             
	);
	return hists;
}
