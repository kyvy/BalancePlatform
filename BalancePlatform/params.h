#pragma once

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#define CV_WAIT_TIME 1
#define CONFIG_PATH "config.txt"
#define ARDUINO_PORT "\\\\.\\COM3"

#define SERVO_MIN 1000.0f  // minimum servo position
#define SERVO_MAX 2000.0f  // maximum servo position
#define SERVO_MID 1500.0f  // 


class Params {
public:
	Params(std::string path) { read(path); }

	void read(std::string path);
	void write(std::string path);

	static struct Data: std::map <std::string, std::string> {
		bool iskey(const std::string& s) const { return count(s) != 0; }
	} data;

	static cv::Size size;  // camera size

};
