// Jonathan Ho

// Office Hour Visit

// This is the cpp file for my implementation of a student

#include "student.h"

#include <string>
using namespace std;

// checks if student is default (for debugging purposes)
bool student::isDefault() const {
	return (arrivalTime_ == 0 && startTime_ == 0 && length_ == 0 && urgency_ == 0);
}


// generates arrival or meeting logs for this specific office visit
string student::report(bool arrLog) const {
	if (isDefault()) {
		return "";
	}

	string log = "\t";
	if (arrLog) {
		log += name_ + " is arriving at " + std::to_string(arrivalTime_) + " minutes. The urgency of the meeting is " + std::to_string(urgency_) + " out of 10.\n\n";
	} else {
		if (startTime_ - arrivalTime_ > 0) {
			log += name_ + " has been waiting in the queue for " + std::to_string(startTime_ - arrivalTime_) + " minutes.\n\t";
		}
		log += name_ + " is meeting the professor for " + std::to_string(length_) + " minutes, starting at " + std::to_string(startTime_) + " minutes and ending at " + std::to_string(startTime_ + length_) + " minutes.\n\t";
		log += "The subject of this meeting is " + topic_ + ".\n\t";
		log += "The urgency of this meeting is " + std::to_string(urgency_) + " out of 10.\n\n";
	}
	return log;
}
