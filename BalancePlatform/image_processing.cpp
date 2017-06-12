#include "image_processing.h"


cv::Mat ImageProcessing::rangeThreshold(const cv::Mat& imgBGR, const cv::Vec3i& min, const cv::Vec3i& max) {
	cv::Mat imgHSV, threshold;

	cvtColor(imgBGR, imgHSV, CV_BGR2HSV);
	inRange(imgHSV, cv::Scalar(min), cv::Scalar(max), threshold);
	return threshold;
}


std::vector<cv::Point> ImageProcessing::mergeContours(std::vector<std::vector<cv::Point>> contours) {
	std::vector<cv::Point> merged;

	for (size_t i=0; i<contours.size(); i++)
		for (size_t j=0; j<contours[i].size(); j++)
			merged.push_back(contours[i][j]);
	
	return merged;
}


void ImageProcessing::morph(cv::Mat src, cv::Mat dst, int erodeSize, int dilateSize) {
	cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erodeSize + 1, 2 * erodeSize + 1));
	cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * dilateSize + 1, 2 * dilateSize + 1));

	cv::dilate(src, dst, dilateKernel);
	cv::erode(dst, dst, erodeKernel);
}