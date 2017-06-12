#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "params.h"
#include "serial.h"
#include "utility.h"


class Control {
private:
	Serial arduino;
	char input[MAX_LENGTH];     // string read from arduino

	cv::Point2f position, integral, derivative, control;
	cv::Point2f prevError, error, scrsize;
	float delta{1.0f}, KP{0}, KI{0}, KD{0};

public:
	Control() { 
		importParams(); 
		scrsize = { (float)Params::size.width, (float)Params::size.height };
	}

	void importParams();
	void exportParams();

	void reset();

	void update(cv::Rect roi, cv::Point2f position, cv::Point2f target={0,0});

	Serial& getArduino() { return arduino; }
};

