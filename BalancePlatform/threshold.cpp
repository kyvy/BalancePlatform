#include "threshold.h"


Threshold::Threshold(std::string winname): winname(winname){
	importParams();
	createWindow();
}

void Threshold::createWindow() {
	cv::namedWindow(winname, CV_WINDOW_AUTOSIZE);

	cv::createTrackbar("H min", winname, &min[0], 179);
	cv::createTrackbar("H max", winname, &max[0], 179);
	cv::createTrackbar("S min", winname, &min[1], 255);
	cv::createTrackbar("S max", winname, &max[1], 255);
	cv::createTrackbar("V min", winname, &min[2], 255);
	cv::createTrackbar("V max", winname, &max[2], 255);

	cv::createTrackbar("erodeSize", winname, &erodeSize, 30);
	cv::createTrackbar("dilateSize", winname, &dilateSize, 30);
}

void Threshold::importParams() {
	try {
		min[0] = std::stoi(Params::data[winname + "/H min"]);
		max[0] = std::stoi(Params::data[winname + "/H max"]);
		min[1] = std::stoi(Params::data[winname + "/S min"]);
		max[1] = std::stoi(Params::data[winname + "/S max"]);
		min[2] = std::stoi(Params::data[winname + "/V min"]);
		max[2] = std::stoi(Params::data[winname + "/V max"]);

		erodeSize = std::stoi(Params::data[winname + "/erodeSize"]);
		dilateSize = std::stoi(Params::data[winname + "/dilateSize"]);
	} catch (...) {}
}

void Threshold::exportParams() {
	Params::data[winname + "/H min"] = std::to_string(min[0]);
	Params::data[winname + "/H max"] = std::to_string(max[0]);
	Params::data[winname + "/S min"] = std::to_string(min[1]);
	Params::data[winname + "/S max"] = std::to_string(max[1]);
	Params::data[winname + "/V min"] = std::to_string(min[2]);
	Params::data[winname + "/V max"] = std::to_string(max[2]);

	Params::data[winname + "/erodeSize"] = std::to_string(erodeSize);
	Params::data[winname + "/dilateSize"] = std::to_string(dilateSize);
}

void Threshold::update(cv::Mat frameBGR) {
	thresh = ImageProcessing::rangeThreshold(frameBGR, min, max);
	ImageProcessing::morph(thresh, thresh, erodeSize, dilateSize);
	cv::imshow(winname, thresh);
}


