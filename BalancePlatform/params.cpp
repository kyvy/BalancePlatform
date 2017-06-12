#include "params.h"


Params::Data Params::data;
cv::Size Params::size;


std::istream& operator>>(std::istream& is, Params& params) {
	std::string s, key, value;

	while (std::getline(is, s)) {
		std::string::size_type begin = s.find_first_not_of(" \f\t\v");

		if (begin == std::string::npos) continue;                            // Skip blank lines
		if (std::string("#").find(s[begin]) != std::string::npos) continue;  // Skip comments

		// Extract the key
		std::string::size_type end = s.find(':', begin);
		key = s.substr(begin, end - begin);

		key.erase(key.find_last_not_of(" \f\t\v") + 1);  // remove trailing whitespaces
		if (key.empty()) continue;

		// Extract the value (no leading or trailing whitespace allowed)
		begin = s.find_first_not_of(" \f\n\r\t\v", end + 1);
		end = s.find_last_not_of(" \f\n\r\t\v") + 1;

		Params::data[key] = s.substr(begin, end - begin);
	}

	return is;
}


std::ostream& operator<<(std::ostream& os, Params& params) {
	std::map <std::string, std::string>::const_iterator iter;
	for (iter = params.data.begin(); iter != params.data.end(); iter++)
		os << iter->first << ": " << iter->second << std::endl;

	return os;
}


void Params::read(std::string path) {
	std::ifstream input(path);
	std::istream& is = input;

	is >> *this;
}


void Params::write(std::string path) {
	std::ofstream output(path);
	output << *this << std::endl;
}