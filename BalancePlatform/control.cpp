#include "control.h"


static cv::Point2f& operator*(cv::Point2f& lhs, const cv::Point2f& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}


static cv::Point2f& operator/(cv::Point2f& lhs, const cv::Point2f& rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	return lhs;
}


static cv::Point2f& operator+(cv::Point2f& lhs, const float& rhs) {
	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}


void Control::update(cv::Rect roi, cv::Point2f position, cv::Point2f target) {
	//std::cout << position << std::endl;
	cv::Point2f framesize = { (float)roi.width, (float)roi.height };

	// transform position and target to servo 'coordinates'
	// target assumed to be [-1.0, 1.0]
	position = (position / framesize) * (SERVO_MAX-SERVO_MIN) + SERVO_MIN;
	target = ((target + 1.0)/2.0) * (SERVO_MAX-SERVO_MIN) + SERVO_MIN;

	error = target - position;

	integral += error*delta;
	derivative = (error-prevError)/delta;

	control = KP*error + KD*derivative + KI*integral + SERVO_MID;
	prevError = error;

	control = Utility::clamp2(control, SERVO_MIN, SERVO_MAX);

	//std::cout << position << std::endl;
	//std::cout << error << std::endl;
	std::cout << control << std::endl;
	//std::cout << target << std::endl;
	//std::cout << std::endl;
	
	if (!arduino.isConnected()) { std::cout << "Arduino disconnected" << std::endl; return; }

	std::ostringstream ss;
	ss << "<" << (int)control.x << "," << (int)control.y << ">";
	std::string out = ss.str();

	std::cout << out << std::endl;

	char* outcstr = new char[out.size()];
	std::copy(out.begin(), out.end(), stdext::checked_array_iterator<char*>(outcstr, out.size()));

	arduino.write(outcstr, MAX_LENGTH);
	delete[] outcstr;
}


void Control::importParams() {
	try {
		KP = std::stof(Params::data["KP"]);
		KI = std::stof(Params::data["KI"]);
		KD = std::stof(Params::data["KD"]);

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		KP = 1.0;
		KI = 1.0;
		KD = 1.0;
		delta = 1.0;
	}
}


void Control::exportParams() {
	Params::data["KP"] = std::to_string((double)KP);
	Params::data["KI"] = std::to_string((double)KI);
	Params::data["KD"] = std::to_string((double)KD);
}

void Control::reset() {
	control = { SERVO_MID, SERVO_MID };
	error = { 0,0 };
	prevError = { 0,0 };
}