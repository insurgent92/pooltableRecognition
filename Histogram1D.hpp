#pragma once
#include "use_opencv.h"

class Histogram1D {
private:

	int histSize[1]; // �󵵼�
	float hranges[2]; // �ּ�/�ִ� ȭ�Ұ�
	const float* ranges[1];
	int channels[1]; // ���⼭ 1ä�θ� ���

public:
	Histogram1D();

	// ������ ��� ������ �׷��̷��� ������ ������׷��� ����� ���� ���� �޼ҵ带 ����� ����
	std::vector<cv::MatND> getHistogram(const cv::Mat &image);

};