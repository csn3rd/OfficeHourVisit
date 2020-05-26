# Office Hour Visit - Jonathan Ho

This is an office hour simulation. Students will arrive at a random time and will meet with the professor for a random amount of time. Student names and subjects are generated from "Subjects.txt" and "Student Roster.txt" respectively. By default, the program runs 100 simulations.

Feel free to add subjects to "Subjects.txt" and names to "Student Roster.txt". Plus, feel free to change the "numberOfSimulations" (line 26 of main.cpp), how often students arrive (lines 105 and 133), or how long students meet with the professor (line 114). You can set the program to 10,000 simulations if you would like. Just make sure that you have enough memory for it; haha (it'll make over 20,000 files).

I used the C++ STL Queue to keep track of arriving students, the C++ STL Priority_Queue to keep track of students that are currently in line to meet with the professor, and the C++ STL Multimap to record and generate reports on what each student attended office hours for. I then used my own implementation of insertion sort to generate the sorted report.

To compile the code, open terminal/command prompt to this directory and send the following command: "g++ -std=c++11 main.cpp student.cpp -o main". This will make an executable file called main.

After the code has compiled, you can execute the executable file that was just created by sending the following command: "./main".

This should run the program.

***Sample Output***

```
> ./main
The average office hour
	has 11.490000 students
	is 73.140000 minutes long.
	runs 13.360000 minutes overtime.
	has the professor waiting for 11.060000 minutes.
	has each student waiting for 7.472585 minutes.
	has 5.402959 minute long meetings.

The average student
	has visited office hours 32.828571 times.
	has waited for 245.314286 total minutes.
	has met with the professor for 177.371429 total minutes.

Please enter whether you would like to sort by name or subject [N/S]: S

```

In addition, the arrival logs, meeting logs, and student reports folders will contain the corresponding data.

The arrival logs will contain "numberOfSimulations" number of files, each containing arrival information for that specific simulation.

The meeting logs will contain "numberOfSimulations" number of files, each containing meeting information for that specific simulation.

The student reports will contain as many files as there are lines in "Student Roster.txt", each containing the subjects for which the student has asked about.

In the end, an overarching sorted report is generated. The user can specify which field to sort by. 
