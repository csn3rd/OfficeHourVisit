// Jonathan Ho

// Office Hour Visit

// This is the header file for my implementation of a student

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class student {
public:

	// default constructor with no arguments
	student() {
		name_ = "";
		topic_ = "";
		arrivalTime_ = 0;
		startTime_ = 0;
		length_ = 0;
		urgency_ = 0;
	}

	// constructor with arguments
	student(string n, string t, int a, int l, int u) {
		name_ = n;
		topic_ = t;
		arrivalTime_ = a;
		startTime_ = 0;
		length_ = l;
		urgency_ = u;
	}

	// getters / accessors

	string name() const {
		return name_;
	}

	string topic() const {
		return topic_;
	}

	int arrivalTime() const {
		return arrivalTime_;
	}

	int startTime() const {
		return startTime_;
	}

	int length() const {
		return length_;
	}

	int urgency() const {
		return urgency_;
	}

	string report(bool arrLog) const;

	bool isDefault() const;

	// setters / mutators

	void setStartTime(int t) {
		startTime_ = t;
	}

	// comparison operators
	bool operator < (const student &other) const {
		return urgency_ < other.urgency();		// compares the urgency values of two students
	}

private:

	string name_;		// student name

	string topic_;		// the topic of the meeting
	int arrivalTime_;	// time at which student arrives
	int startTime_;		// time at which meeting begins
	int length_;		// the length of the meeting
	int urgency_;		// the urgency of the meeting
};

#endif // STUDENT_H
