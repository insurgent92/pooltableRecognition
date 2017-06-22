#include "use_opencv.h"
#include <iostream>

using namespace std;
using namespace cv;
int main(){

	VideoCapture cap("samples/sample1.avi", IMREAD_GRAYSCALE);
	if (!cap.isOpened())  // 비디오 파일을 여는 데 실패했다면, 프로그램을 종료한다.
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	double fps = cap.get(CV_CAP_PROP_FPS); // 비디오를 초당 몇 프레임 읽어올 것인지 설정한다

	cout << "Frame per seconds : " << fps << endl;
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

	for (;;){
		Mat frame;

		bool bSuccess = cap.read(frame); // 비디오로부터 새 프레임을 읽어온다.


		if (!bSuccess) // 새 프레임을 읽어오는 데 실패했다면, 루프(while문)를 빠져나간다.
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		imshow("MyVideo", frame); // 'MyVideo' 윈도우 창에 읽어온 프레임을 보여준다.

		
		Mat srcImage;
		cvtColor(frame, srcImage, CV_BGR2GRAY);
		Mat edges;
		Mat edges2;
		Canny(srcImage, edges, 50, 100);
		edges = ~edges;
		cvtColor(edges, edges2, CV_GRAY2BGR);
		Scalar loDiff = Scalar::all(10);
		Scalar upDiff = Scalar::all(10);
		int floodFlag = 4 | FLOODFILL_FIXED_RANGE;
		Rect boundRect;
		cv::Mat element5(5, 5, CV_8U, cv::Scalar(255));
		cv::morphologyEx(edges2, edges2, cv::MORPH_ERODE, element5);
		//cv::morphologyEx(edges2, edges2, cv::MORPH_ERODE, element5);
		Mat mask = Mat(Size(edges2.cols + 2, edges2.rows + 2), CV_8U, Scalar::all(0));

		floodFill(edges2, mask, Point(300, 100), Scalar(0, 0, 255), &boundRect, loDiff, upDiff, floodFlag);
		/*cv::Mat element4(10, 10, CV_8U, cv::Scalar(1));
		cv::morphologyEx(mask, mask, cv::MORPH_DILATE, element4);
		cv::morphologyEx(mask, mask, cv::MORPH_DILATE, element4);
		cv::morphologyEx(mask, mask, cv::MORPH_DILATE, element4);


		cv::morphologyEx(mask, mask, cv::MORPH_ERODE, element4);
		cv::morphologyEx(mask, mask, cv::MORPH_ERODE, element4);
		cv::morphologyEx(mask, mask, cv::MORPH_ERODE, element4);*/

		//Mat test = srcImage(mask);

		imshow("edges", edges2);
		//imshow("test", test);



		if (waitKey(30) == 27) // 영상이 재생되는 시간(30초) 동안 'esc' 키를 기다린다.
			//30초 내에 입력되는 경우 루프를 빠져나간다.
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}




	return 0;
}