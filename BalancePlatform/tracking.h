#pragma once
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <algorithm>

#include "image_processing.h"
#include "params.h"


class Tracking {
private:
	cv::RNG rng;

	struct {
		const int N{2};
		cv::Mat edges;

		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Point> hull;                  // convex hull
		std::vector<std::vector<cv::Point>> largest;  // top N largest area contours
		std::vector<cv::Point> merged;                // merged top N largest area contours
		std::vector<cv::Vec4i> hierarchy;

		cv::Moments mu;  // hull moment
		cv::Point2f mc;  // hull mass center
	} ball;

	struct {
		cv::Mat edges;

		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Point> contour;
		std::vector<cv::Vec4i> hierarchy;

		cv::Rect rect;

	} platform;

	cv::Mat cropped;

public:
	Tracking();

	void findBall(const cv::Mat& ballThresh);
	void findPlatform(cv::Mat& platformThresh);

	void display(const cv::Mat& frame);

	cv::Mat& getCropped() { return cropped; }
	cv::Point2f& getCenter() { return ball.mc; }
	cv::Rect& getROI() { return platform.rect; }
};

