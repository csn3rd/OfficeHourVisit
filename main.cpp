// Jonathan Ho

// Office Hour Visit

#include <iostream>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#include <queue>
#include <set>
#include <vector>
#include <map>

#include <iterator>

#include "student.h"

using namespace std;

// set HOUR to 60 minutes
const int HOUR = 60;

int numSimulations = 100;		// total number of simulations

// use set to remove duplicate students and subjects
set<string> listOfNames;
set<string> listOfSubjects;

void simulate();		// runs one office hour simulation
void collateRes();		// collates and analyzes all the simulations run

void nameInput();		// function to read "Student Roster.txt"
void subjectInput();	// function to read "Subjects.txt"

void sort();

string arrivallog;		// string to keep track of arrival data
string meetinglog;		// string to keep track of meeting data


// Variables to track data over all simulations
int totalstudents = 0;
int totalstudentwaittime = 0;
int totalprofessorwaittime = 0;
int totalmeetingtime = 0;
int totalofficetime = 0;
int totalovertime = 0;

multimap<string, string> totalmeetingdata;		// multimap to store subjects that each student has asked about

vector<student> data;		// vector to store all the students for sorting purposes

// main
int main() {

	nameInput();		// get input for names
	subjectInput();		// get input for subjects

	// randomize the seed based on the time
	srand (time(NULL));

	// run the individual simultaions
	for (int i = 0; i < numSimulations; i++) {
		arrivallog = "";		// initialize the arrival log for the current simulation
		meetinglog = "";		// initialize the meeting log for the current simulation
		simulate();		// call the simulation function

		// set the file name to print the arrival log within the corresponding folder
		string filepath = "arrivallogs";
		filepath += "/";
		filepath += "log" + std::to_string(i+1) + ".txt";

		// print to the file
		ofstream alogprint (filepath);
		if (alogprint.is_open()) {
			alogprint << "Arrival Log: " << endl << endl << arrivallog;
			alogprint.close();
		}

		// set the file name to print the meeting log within the corresponding folder
		filepath = "meetinglogs";
		filepath += "/";
		filepath += "log" + std::to_string(i+1) + ".txt";

		// print to the file
		ofstream mlogprint (filepath);
		if (mlogprint.is_open()) {
			mlogprint << "Meeting Log: " << endl << endl << meetinglog;
			mlogprint.close();
		}
	}

	collateRes();		// call the collate results function

	sort();				// call the sort function

	return 0;
}


// simulation function
void simulate() {
	queue<student> students;		// create a queue of students to keep track of arrivals

	int previousArrival = 0;		// variable to keep track of when the last person arrived
	string previousName = "";		// variable to keep track of who the last person to arrive was
	int arrivalRate = rand()%11;		// variable that determines the time gap between arriving students
										// (between 0 and 10 minutes)

	// loop to add all the students who arrive within the time period
	while(previousArrival + arrivalRate < HOUR) {

		// randomize the student's name, topic, meeting length, and urgency
		string name = *std::next(listOfNames.begin(), rand()%(listOfNames.size()));
		string topic = *std::next(listOfSubjects.begin(), rand()%(listOfSubjects.size()));
		int serviceRate = rand()%10 + 1;		// meetings are between 1 and 10 minutes long
		int urgencyRate = rand()%10 + 1;		// the urgency of a meeting are between 1 and 10

		// create the newly arriving student with the randomized values
		student newArrival(name, topic, previousArrival+arrivalRate, serviceRate, urgencyRate);

		// add updates to the arrival log string
		if (students.size() > 0) {
			arrivallog += "\t" + name + " is arriving " + std::to_string(arrivalRate) + " minutes after " + previousName + ".\n";
		}
		arrivallog += newArrival.report(true);

		// update the previous arrival time and previous student name to the newly arriving student
		previousArrival += arrivalRate;
		previousName = name;

		// push the newly arriving student to the end of the queue
		students.push(newArrival);

		arrivalRate = rand()%11;		// determine the next time gap
	}

	priority_queue<student> queue;		// create a priority queue to keep track of students currently waiting

	int ohlength = 0;		// variable to keep track of the lenght of the office hour
	int count = 0;		// variable to keep track of the number of students so far (mainly used for printing and debugging purposes)

	// put all the students through the office hour
	while (!queue.empty() || !students.empty()) {
		if (queue.empty()) {					// this is the case where the professor has to wait for students to arrive
			// take students from the front of the arrival queue and push them into the priority queue
			queue.push(students.front());
			students.pop();
		} else {								// this is the case where there are students waiting for their meeting
			// take student from the top of the priority queue (most urgent)
			student current = queue.top();
			queue.pop();

			data.push_back(current);		// add current student to the vector to sort later

			// increment trackers
			count++;
			totalstudents++;

			// update meeting logs accordingly
			if (ohlength < current.arrivalTime()) {
				meetinglog += "\tThe professor has been waiting for " + std::to_string(current.arrivalTime() - ohlength) + " minutes before " + current.name() + " showed up.\n";
				totalprofessorwaittime += current.arrivalTime() - ohlength;		// increment the professor wait time tracker
			} else if (count != 1 && ohlength == current.arrivalTime()) {
				meetinglog += "\t" + current.name() + " arrived just as the previous student was leaving.\n";			
			} else if (count == 1 && ohlength == current.arrivalTime()) {
				meetinglog += "\t" + current.name() + " arrived just as the office hour was starting.\n";
			} else {
				totalstudentwaittime += ohlength-current.arrivalTime();		// increment the student wait time tracker
			}

			// the current student's office hour begins either at the end of the professor's previous meeting or when the student arrives, whichever comes later
			ohlength = std::max(ohlength, current.arrivalTime());
			current.setStartTime(ohlength);

			meetinglog += current.report(false);		// update meeting logs accordingly

			totalmeetingtime += current.length();		// increment the total meeting time tracker

			ohlength = current.startTime() + current.length();		// update the office hour time to after the current meeting is over

			totalmeetingdata.insert(pair<string, string> (current.name(), current.topic()));		// add the current meeting data to the multimap

			// add students that have arrived to the priority queue
			while(!students.empty() && students.front().arrivalTime() <= ohlength) {
				queue.push(students.front());
				students.pop();
			}
		}
	}

	totalofficetime += ohlength;		// increment the total office time tracker

	// increment the total overtime tracker
	if (ohlength > HOUR) {
		totalovertime += (ohlength-HOUR);
	}
}

// insertion sort function
void sort() {

	// input
	cout << "\nPlease enter whether you would like to sort by name or subject [N/S]: ";
	string sortingBy;
	cin >> sortingBy;

	// determine field to sort by
	bool sortingByName = true;
	if (sortingBy == "S") {
		sortingByName = false;
	}

	// insertion sort
	for (int i = 0; i < data.size(); i++) {
		int j = i-1;
		student current = data[i];
		if (sortingByName) {
			while (j >= 0 && (current.name() < data[j].name() || (current.name() == data[j].name() && current.topic() < data[j].topic()))) {
				data[j+1] = data[j];
				j--;
			}
			data[j+1] = current;
		} else {
			while (j >= 0 && (current.topic() < data[j].topic() || (current.topic() == data[j].topic() && current.name() < data[j].name()))) {
				data[j+1] = data[j];
				j--;
			}
			data[j+1] = current;
		}
	}

	// print to file
	ofstream reportPrint ("Sorted Report.txt");
	if (reportPrint.is_open()) {
		for (auto const& i: data) {
			if (sortingByName) {
				reportPrint << i.name() << " " << i.topic() << endl;
			} else {
				reportPrint << i.topic() << " " << i.name() << endl;
			}
		}
	}
}

// collate results function
void collateRes() {

	// print the overall averages
	cout << "The average office hour" << endl;
	cout << "\thas " << to_string(totalstudents/(double)numSimulations) << " students" << endl;
	cout << "\tis " << to_string(totalofficetime/(double)numSimulations) << " minutes long." << endl;
	cout << "\truns " << to_string(totalovertime/(double)numSimulations) << " minutes overtime." << endl;
	cout << "\thas the professor waiting for " << to_string(totalprofessorwaittime/(double)numSimulations) << " minutes." << endl;
	cout << "\thas each student waiting for " << to_string(totalstudentwaittime/(double)totalstudents) << " minutes." << endl;
	cout << "\thas " << to_string(totalmeetingtime/(double)totalstudents) << " minute long meetings." << endl;

	cout << "\nThe average student" << endl;
	cout << "\thas visited office hours " << to_string(totalstudents/(double)listOfNames.size()) << " times." << endl;
	cout << "\thas waited for " << to_string(totalstudentwaittime/(double)listOfNames.size()) << " total minutes." << endl;	
	cout << "\thas met with the professor for " << to_string(totalmeetingtime/(double)listOfNames.size()) << " total minutes." << endl;


	// go through the student roster and generate reports for each student
	for (int i = 0; i < listOfNames.size(); i++) {

		// set the file name to print the student report within the corresponding folder
		string filepath = "studentreports";
		filepath += "/";

		string name = *std::next(listOfNames.begin(), i);		// name of specific student

		filepath += name + ".txt";

		// print to the file		
		ofstream reportPrint (filepath);
		if (reportPrint.is_open()) {
			reportPrint << "Subjects that " + name + " has attended office hours for:\n" << endl;
			for (auto j = totalmeetingdata.lower_bound(name); j != totalmeetingdata.upper_bound(name); j++) {
				reportPrint << j -> second << endl;
			}
			reportPrint.close();
		}
	}
}


// read the student roster file
void nameInput() {

	string name;
	ifstream nameReader ("Student Roster.txt");

	if (nameReader.is_open()) {
		while(getline(nameReader,name)) {
			listOfNames.insert(name);
		}
	}

	nameReader.close();

	// debugging function to check if names are read properly
	/*
	for (auto i = listOfNames.begin(); i != listOfNames.end(); i++) {
		cout << *i << " ";
	}
	*/
}


// read the subject file
void subjectInput() {
	
	string subject;
	ifstream subjectReader ("Subjects.txt");

	if (subjectReader.is_open()) {
		while(getline(subjectReader, subject)) {
			listOfSubjects.insert(subject);
		}
	}

	subjectReader.close();

	// debugging function to check if subjects are read properly
	/*
	for (auto i = listOfSubjects.begin(); i != listOfSubjects.end(); i++) {
		cout << *i << " ";
	}
	*/
}
