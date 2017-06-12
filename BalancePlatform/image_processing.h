#pragma once
#include <opencv2/opencv.hpp>


class ImageProcessing {
public:
	static cv::Mat rangeThreshold(const cv::Mat& imgBGR, const cv::Vec3i& min, const cv::Vec3i& max);

	static std::vector<cv::Point> mergeContours(std::vector<std::vector<cv::Point>> contours);

	static void morph(cv::Mat src, cv::Mat dst, int erodeSize, int dilateSize);
};
