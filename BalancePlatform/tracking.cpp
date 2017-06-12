#include "tracking.h"

// area comparison, descending
static bool area_compare(const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
	return cv::contourArea(a) > cv::contourArea(b);
}


// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle(const cv::Point& pt1, const cv::Point& pt2, const cv::Point& pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}



Tracking::Tracking() {
	cv::namedWindow("Tracking", CV_WINDOW_AUTOSIZE);
}


void Tracking::findBall(const cv::Mat& ballThresh) {
	ball.largest.clear();

	cv::Canny(ballThresh(platform.rect), ball.edges, 100, 300, 3);
	cv::findContours(ball.edges, ball.contours, ball.hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	std::sort(ball.contours.begin(), ball.contours.end(), area_compare);

	ball.largest.assign(ball.contours.begin(), ball.contours.begin() + std::min((int)ball.contours.size(), ball.N));
	ball.merged = ImageProcessing::mergeContours(ball.largest);

	if (ball.merged.empty()) {
		ball.mc = { (float)platform.rect.width / 2, (float)platform.rect.height / 2 };
		return;
	}

	cv::convexHull(ball.merged, ball.hull);

	ball.mu = cv::moments(ball.hull);
	ball.mc = cv::Point2f(ball.mu.m10 / ball.mu.m00, ball.mu.m01 / ball.mu.m00);  // calculate mass centers
}


void Tracking::findPlatform(cv::Mat& platformThresh) {
	cv::Mat res = cv::Mat::zeros(platformThresh.size(), CV_8UC3);

	cv::Canny(platformThresh, platform.edges, 100, 300, 3);

	cv::findContours(platform.edges, platform.contours, platform.hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	if (platform.contours.empty()) return;

	platform.contour = *std::max_element(platform.contours.begin(), platform.contours.end(), area_compare);
	platform.rect = cv::boundingRect(platform.contour);

	cv::rectangle(platformThresh, platform.rect, cv::Scalar(0,0,255));
	cv::imshow("Tracking", platformThresh(platform.rect));
}


void Tracking::display(const cv::Mat& frame) {
	cv::Mat croppedFrame = frame(platform.rect);
	auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

	cv::polylines(croppedFrame, ball.largest, false, color);
	cv::polylines(croppedFrame, ball.hull, false, color);
	cv::circle(croppedFrame, ball.mc, 1, color, -1, 8, 0);

	cv::imshow("Tracking", croppedFrame);
}


