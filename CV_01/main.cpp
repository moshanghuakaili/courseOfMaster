

#include <opencv2\opencv.hpp>

using namespace cv;

#include <iostream>
int main()
{
	int width = 1080;
	int height = 720;
	
	VideoCapture capture("Source/kepler.mp4");
	double rate = ceil(capture.get(CV_CAP_PROP_FPS)); //获取视频帧率
	VideoWriter writer;
	Size videoSize(width, height);
	writer.open("CV_HOMEWORK_1.avi", CV_FOURCC('M', 'J', 'P', 'G'), rate, videoSize);
	//片头制作
	Mat picture(height, width, CV_8UC3, Scalar(255, 255, 255));
	
	RNG rng(0xFFFFFFFF);
	
	for (int i = 0; i < 85; i++) {
		int color_i = unsigned(rng);
		Scalar color = Scalar(color_i & 255, (color_i >> 8) & 255, (color_i >> 16) & 255);
		Point point1, point2;
		int house[6][2] = { { 0,200 },{ 50,0 },{ 0,50 },{ 50,0 },{ 0,-250 },{ 1080,0 } };
		int house1[6][2] = { { 0,500 },{ 100,0 },{ 0,20 },{ 20,0 },{ 0,-350 },{ 1080,0 } };
		int house2[6][2] = { { 0,100 },{ 90,0 },{ 0,-100 },{ 300,0 },{ 0,70 },{ 1080,0 } };
		int house3[6][2] = { { 0,300 },{ 250,0 },{ 50,50 },{ 50,-50 },{ 0,-250 },{ 1080,0 } };
		point1.x = 0;
		point1.y = point2.y = rng.uniform(3 * height / 4, height - 2);
		point2.x = rng.uniform(1, width / 2);
		line(picture, point1, point2, color, 2, CV_AA);//绘制线条
		putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
		imshow("CV_HOMEWORK_1", picture);

		writer << picture;

		waitKey(25);
		if (rng.uniform(1, 4) == 1) {
			for (int i = 0; i < 6; i++) {
				point1.x = point2.x;
				point1.y = point2.y;
				if (house[i][0] != 0) {
					point2.x = point2.x + house[i][0] + rng.uniform(1, 10);
				}
				if (house[i][1] != 0) {
					point2.y = point2.y - house[i][1] + rng.uniform(1, 10);;
				}
				line(picture, point1, point2, color, 2, CV_AA);
				putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", picture);

				writer << picture;

				waitKey(25);
			}
		}
		else if (rng.uniform(1, 4) == 3) {
			for (int i = 0; i < 6; i++) {
				point1.x = point2.x;
				point1.y = point2.y;
				if (house1[i][0] != 0) {
					point2.x = point2.x + house1[i][0] + rng.uniform(1, 10);
				}
				if (house1[i][1] != 0) {
					point2.y = point2.y - house1[i][1] + rng.uniform(1, 10);;
				}
				line(picture, point1, point2, color, 2, CV_AA);
				putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", picture);

				writer << picture;

				waitKey(25);
			}
		}
		else if (rng.uniform(1, 4) == 2) {
			for (int i = 0; i < 6; i++) {
				point1.x = point2.x;
				point1.y = point2.y;
				if (house2[i][0] != 0) {
					point2.x = point2.x + house2[i][0] + rng.uniform(1, 10);
				}
				if (house2[i][1] != 0) {
					point2.y = point2.y - house2[i][1] + rng.uniform(1, 10);;
				}
				line(picture, point1, point2, color, 2, CV_AA);
				putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", picture);

				writer << picture;

				waitKey(25);
			}
		}
		else {
			for (int i = 0; i < 6; i++) {
				point1.x = point2.x;
				point1.y = point2.y;
				if (house3[i][0] != 0) {
					point2.x = point2.x + house3[i][0] + rng.uniform(1, 10);
				}
				if (house3[i][1] != 0) {
					point2.y = point2.y - house3[i][1] + rng.uniform(1, 10);;
				}
				line(picture, point1, point2, color, 2, CV_AA);
				putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", picture);

				writer << picture;

				waitKey(25);
			}
		}
	
		Point textpos;
		textpos.x = 80;
		
		if (i > 15 && i < 22) {
			textpos.y = 120;
			putText(picture, "The vastness of the universe ...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 22 && i < 29) {
			textpos.y = 160;
			putText(picture, "The hustle of the city ...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 29 && i < 36) {
			textpos.y = 200;
			putText(picture, "The tiny things ...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1.5, 8, 0);
			
		}
		if (i > 36 && i < 43) {
			textpos.y = 240;
			putText(picture, "The great things ...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1.5, 8, 0);
			
		}
		if (i > 43 && i < 50) {
			textpos.y = 280;
			putText(picture, "Disappointment and expectation ...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 50 && i < 57) {
			textpos.y = 320;
			putText(picture, "Loneliness and companionship ...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 57 && i < 64) {
			textpos.y = 360;
			putText(picture, "But...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 64 && i < 71) {
			textpos.y = 400;
			putText(picture, "...", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 71) {
			textpos.y = 360;
			putText(picture, "....  Kepler By:Stefanie Sun ....", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 2.2, cv::Scalar(50, 50, 50), 1, 8, 0);
			
		}
		if (i > 78) {
			textpos.y = 480;
			putText(picture, "           Made by LiTing (21721079) ", textpos, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(50, 50, 50), 1, 8, 0);
		
		}
		blur(picture, picture, Size(3, 3));
	}
	
	
	for (int k = 0; k < 20; k++) {
		erode(picture, picture, getStructuringElement(MORPH_RECT, Size(3, 3)));
		putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
		imshow("CV_HOMEWORK_1", picture);
		
		writer << picture;

		waitKey(20);
	}
	putText(picture, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
	imshow("CV_HOMEWORK_1", picture);
	

	//展示图片
	Mat img;
	for (int i = 1; i<9; i++)
	{
		char filename[20];
		sprintf_s(filename, "Source/pic%d.jpg", i);
		img = imread(filename);//导入图片
		resize(img, img, Size(width, height), (0, 0), (0, 0), 3);

		//腐蚀出现
		//模糊变黑消失
		if (i%2) {
			Mat erode_img;
			Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
			for (int k = 15; k > 0; k--) {
				erode(img, erode_img, element);
				for (int j = 0; j < k; j++) {
					erode(erode_img, erode_img, element);
				}
				putText(erode_img, " Made by LiTing (21721079) ", Point(350,690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				
				writer << erode_img;
				
				imshow("CV_HOMEWORK_1", erode_img);
				waitKey(50);
			}

			Mat blur_img;
			putText(img, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
			imshow("CV_HOMEWORK_1", img);
			for (int p = 0; p < 45; p++) {
				writer << img;
			}
			waitKey(1500);
			blur_img = img;
			int rows = blur_img.rows;
			int cols = blur_img.cols;
			for (int k = 0; k < 20; k++) {
				if (k > 15) {
					for (int l = 0; l < rows; l++)
					{
						for (int j = 0; j < cols; j++)
						{
							blur_img.at<Vec3b>(l, j)[0] = blur_img.at<Vec3b>(l, j)[0] / 1.1;
							blur_img.at<Vec3b>(l, j)[1] = blur_img.at<Vec3b>(l, j)[1] / 1.1;
							blur_img.at<Vec3b>(l, j)[2] = blur_img.at<Vec3b>(l, j)[2] / 1.1;
						}
					}
				}
				blur(blur_img, blur_img, Size(9, 9));
				putText(blur_img, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", blur_img);
				
				writer << blur_img;
			
				waitKey(25);
			}
		}
		//从模糊到清楚出现
		//矩形腐蚀变黑消失
		else {
			Mat blur_img;
			for (int k = 30; k > 0; k--) {
				blur(img, blur_img, Size(9, 9));
				for (int j = 0; j < k; j++) {
					blur(blur_img, blur_img, Size(9, 9));
				}
				putText(blur_img, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", blur_img);
				
				writer << blur_img;
				
				waitKey(20);
			}
			putText(img, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
			imshow("CV_HOMEWORK_1", img);
			for (int p = 0; p < 45; p++) {
				writer << img;
			}
			waitKey(1500);

			Mat erode_img;
			erode_img = img;
			Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
			for (int k = 0; k < 15; k++) {
				erode(erode_img, erode_img, element);
				putText(erode_img, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
				imshow("CV_HOMEWORK_1", erode_img);
				writer << erode_img;
				waitKey(50);
			}
		}
	}
	
	int count = 0;
	while (true) {
		Mat frame;
		Mat edge;

		if (!capture.read(frame)) {//读取视频帧
			break;
		}
		resize(frame, frame, Size(width, height), (0, 0), (0, 0), 3);
		putText(frame, " Made by LiTing (21721079) ", Point(350, 690), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(50, 50, 50), 1, 8, 0);
		imshow("CV_HOMEWORK_1", frame);
		if (count % 2) {
			writer << frame;
		}
		count++;
		
		waitKey(ceil(1000/(rate*2)));//2倍速播放
	}
	return 0;
}