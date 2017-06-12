#pragma once
#include <opencv2/opencv.hpp>


class Utility {
public:
	
	template <typename cvPoint2>
	static cvPoint2 clamp2(cvPoint2 point, cvPoint2 min, cvPoint2 max) {
		if (point.x > max.x) point.x = max.x;
		else if (point.x < min.x) point.x = min.x;

		if (point.y > max.y) point.y = max.y;
		else if (point.y < min.y) point.y = min.y;

		return point;
	}
	
	template <typename cvPoint2>
	static cvPoint2 clamp2(cvPoint2 point, cvPoint2 bounds) {
		// bounds.x -> lower bound
		// bounds.y -> upper bound

		if (point.x > bounds.y) point.x = bounds.y;
		else if (point.x < bounds.x) point.x = bounds.x;

		if (point.y > bounds.y) point.y = bounds.y;
		else if (point.y < bounds.x) point.y = bounds.x;

		return point;
	}

	template <typename cvPoint2, typename BType>
	static cvPoint2 clamp2(cvPoint2 point, BType min, BType max) {
		if (point.x > max) point.x = max;
		else if (point.x < min) point.x = min;

		if (point.y > max) point.y = max;
		else if (point.y < min) point.y = min;

		return point;
	}

};

