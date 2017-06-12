#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "image_processing.h"
#include "params.h"


class Threshold {
private:
	const std::string winname;

	cv::Mat thresh;
	cv::Vec3i min{0,0,0}, max{179,255,255};
	int erodeSize{0}, dilateSize{0};

	void createWindow();

public:
	
	Threshold(std::string winname);

	void update(cv::Mat frame);

	void importParams();
	void exportParams();
	
	cv::Vec3i& getMin() { return min; }
	cv::Vec3i& getMax() { return max; }
	cv::Mat& getThresh() { return thresh; }
};

