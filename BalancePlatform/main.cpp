#include <opencv2/opencv.hpp>
#include <iostream>
#include <valarray>

#include "threshold.h"
#include "tracking.h"
#include "params.h"
#include "control.h"


int main() {
	cv::VideoCapture stream(1);
	if (!stream.isOpened()) { std::cout << "cannot open camera" << std::endl; return -1; }

	//stream.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//stream.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	cv::Mat frameBGR, frameGray;
	stream.read(frameBGR);
	Params::size = frameBGR.size();

	Params PARAMS(CONFIG_PATH);
	Threshold platformThresh("Platfrom Threshold");
	Threshold ballThresh("Ball Threshold");
	Tracking tracking;
	Control control;

	while (true) {
		stream.read(frameBGR);

		cv::blur(frameBGR, frameBGR, cv::Size(5, 5));
		cv::cvtColor(frameBGR, frameGray, CV_BGR2GRAY);

		platformThresh.update(frameBGR);
		tracking.findPlatform(platformThresh.getThresh());

		if (cv::waitKey(CV_WAIT_TIME) == 'b') break;
	}

	cv::destroyWindow("Platfrom Threshold");

	while (true) {
		stream.read(frameBGR);

		cv::blur(frameBGR, frameBGR, cv::Size(5, 5));
		cv::cvtColor(frameBGR, frameGray, CV_BGR2GRAY);
		
		ballThresh.update(frameBGR);

		tracking.findBall(ballThresh.getThresh());
		tracking.display(frameBGR);
		
		control.update(tracking.getROI(), tracking.getCenter());

		if (cv::waitKey(CV_WAIT_TIME) == 'q') break;

		else if (cv::waitKey(CV_WAIT_TIME) == 'r') {
			control.reset();

		}
	}


	platformThresh.exportParams();
	ballThresh.exportParams();
	control.exportParams();
	PARAMS.write(CONFIG_PATH);

	cv::destroyAllWindows();
	return 0;
}

