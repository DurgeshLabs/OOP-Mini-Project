// The same student records written the way we would have written them before
// the OOP unit - global arrays and a set of functions that operate on them.
//
// This file is kept in the repo on purpose. Comparing it with src/main.cpp is
// the clearest way to see what classes actually buy you.
//
// Build:  g++ -std=c++17 -Wall -Wextra -o bin/procedural src/procedural_version.cpp

#include <iomanip>
#include <iostream>
#include <string>

const int MAX_STUDENTS = 100;
const int MAX_MARKS    = 10;

// The "record" is spread across seven parallel arrays. Index 3 of every array
// has to describe the same student, and nothing in the language enforces that.
std::string ids[MAX_STUDENTS];
std::string names[MAX_STUDENTS];
int         ages[MAX_STUDENTS];
std::string emails[MAX_STUDENTS];
std::string courses[MAX_STUDENTS];
std::string types[MAX_STUDENTS];              // "UG" or "PG", as a string
double      marks[MAX_STUDENTS][MAX_MARKS];
int         markCount[MAX_STUDENTS];
int         studentCount = 0;

// Six parameters in a row, all of them strings except one. Swap email and
// course by accident and the compiler will not say a word.
void addStudent(const std::string& id, const std::string& name, int age,
                const std::string& email, const std::string& course,
                const std::string& type) {
    if (studentCount >= MAX_STUDENTS) {
        std::cout << "Student list is full.\n";
        return;
    }
    ids[studentCount]       = id;
    names[studentCount]     = name;
    ages[studentCount]      = age;
    emails[studentCount]    = email;
    courses[studentCount]   = course;
    types[studentCount]     = type;
    markCount[studentCount] = 0;
    studentCount++;
}

int findStudent(const std::string& id) {
    for (int i = 0; i < studentCount; i++)
        if (ids[i] == id) return i;
    return -1;
}

// Nothing stops another part of the program from writing straight into
// marks[i][j] and skipping this check completely.
void addMark(int index, double mark) {
    if (index < 0 || index >= studentCount) return;
    if (mark < 0 || mark > 100) return;
    if (markCount[index] >= MAX_MARKS) return;
    marks[index][markCount[index]] = mark;
    markCount[index]++;
}

double average(int index) {
    if (index < 0 || markCount[index] == 0) return 0.0;
    double total = 0;
    for (int j = 0; j < markCount[index]; j++) total += marks[index][j];
    return total / markCount[index];
}

// Here is the real cost of the procedural approach. Every rule for every kind
// of student lives in one function, chosen by comparing a string. Adding a
// diploma student means editing this function, and the pass-mark function, and
// the display function, and anything else that branches on type.
std::string calculateGrade(int index) {
    double avg = average(index);
    if (types[index] == "PG") {
        if (avg >= 90) return "A+";
        if (avg >= 85) return "A";
        if (avg >= 75) return "B";
        if (avg >= 65) return "C";
        if (avg >= 55) return "D";
        return "F";
    } else {
        if (avg >= 90) return "A+";
        if (avg >= 80) return "A";
        if (avg >= 70) return "B";
        if (avg >= 60) return "C";
        if (avg >= 50) return "D";
        return "F";
    }
}

bool hasPassed(int index) {
    double required = (types[index] == "PG") ? 55.0 : 50.0;   // branch number two
    return average(index) >= required;
}

void updateStudent(const std::string& id, const std::string& newName,
                   const std::string& newCourse) {
    int index = findStudent(id);
    if (index == -1) {
        std::cout << "Student not found.\n";
        return;
    }
    if (!newName.empty())   names[index]   = newName;
    if (!newCourse.empty()) courses[index] = newCourse;
}

void displayAll() {
    std::cout << std::left
              << std::setw(8) << "ID" << std::setw(20) << "NAME"
              << std::setw(10) << "COURSE" << std::setw(6) << "TYPE"
              << std::right << std::setw(9) << "AVERAGE"
              << std::setw(7) << "GRADE" << std::setw(9) << "RESULT" << "\n";
    std::cout << std::string(69, '-') << "\n";

    for (int i = 0; i < studentCount; i++) {
        std::cout << std::left
                  << std::setw(8)  << ids[i]
                  << std::setw(20) << names[i]
                  << std::setw(10) << courses[i]
                  << std::setw(6)  << types[i]
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(9)  << average(i)
                  << std::setw(7)  << calculateGrade(i)
                  << std::setw(9)  << (hasPassed(i) ? "Pass" : "Fail") << "\n";
    }
}

int main() {
    addStudent("P001", "Rahul Verma", 20, "rahul@example.com", "BCA", "UG");
    addMark(0, 80); addMark(0, 85); addMark(0, 90);

    addStudent("P002", "Priya Nair", 21, "priya@example.com", "MCA", "PG");
    addMark(1, 75); addMark(1, 88); addMark(1, 82);

    updateStudent("P001", "Rahul V. Sharma", "");

    std::cout << "Procedural version\n\n";
    displayAll();

    // Note how the data and the functions are completely separate. Any part of
    // the program can reach into these arrays and change them, and every new
    // student type means editing several functions instead of adding a class.
    return 0;
}
