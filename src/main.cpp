// Student Management System
// OOP mini project - encapsulation, inheritance, polymorphism, abstraction.
//
// Build:  make          (or)   g++ -std=c++17 -Wall -Wextra -o sms src/main.cpp
// Run:    ./bin/sms

#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// -------------------------------------------------------------------------
// Input helpers
//
// Everything is read with getline and parsed afterwards. We first wrote this
// with `cin >> age` and then a getline for the name, and the getline kept
// coming back empty because the newline was still sitting in the buffer.
// Reading whole lines and parsing them ourselves avoids that completely.
// -------------------------------------------------------------------------
namespace input {

std::string trim(const std::string& text) {
    const std::string blanks = " \t\r\n";
    std::size_t first = text.find_first_not_of(blanks);
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(blanks);
    return text.substr(first, last - first + 1);
}

// Returns false when the input stream is closed (Ctrl-D / piped input ends).
bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    std::string raw;
    if (!std::getline(std::cin, raw)) return false;
    out = trim(raw);
    return true;
}

// Keeps asking until something is actually typed. Empty string means EOF.
std::string readText(const std::string& prompt) {
    std::string value;
    while (readLine(prompt, value)) {
        if (!value.empty()) return value;
        std::cout << "  This field cannot be blank.\n";
    }
    return "";
}

// Same as readText but an empty answer is allowed (used when updating a
// record, where blank means "leave this field as it is").
std::string readOptional(const std::string& prompt) {
    std::string value;
    if (!readLine(prompt, value)) return "";
    return value;
}

int readInt(const std::string& prompt, int low, int high) {
    std::string line;
    while (readLine(prompt, line)) {
        std::istringstream stream(line);
        int value = 0;
        char leftover = 0;
        if (stream >> value && !(stream >> leftover) && value >= low && value <= high)
            return value;
        std::cout << "  Enter a whole number between " << low << " and " << high << ".\n";
    }
    return low;
}

double readDouble(const std::string& prompt, double low, double high) {
    std::string line;
    while (readLine(prompt, line)) {
        std::istringstream stream(line);
        double value = 0.0;
        char leftover = 0;
        if (stream >> value && !(stream >> leftover) && value >= low && value <= high)
            return value;
        std::cout << "  Enter a number between " << low << " and " << high << ".\n";
    }
    return low;
}

}  // namespace input

// Cuts long text so the table columns stay lined up.
std::string fit(const std::string& text, std::size_t width) {
    if (text.size() <= width - 1) return text;
    return text.substr(0, width - 3) + "..";
}

// =========================================================================
// ABSTRACTION + INHERITANCE
//
// Person is the base class. It only knows what every person has - a name,
// an age and an email. displayInfo() is pure virtual, so Person says "every
// person must be able to describe itself" without deciding how.
// =========================================================================
class Person {
protected:
    std::string name;
    int         age;
    std::string email;

public:
    Person(const std::string& name, int age, const std::string& email)
        : name(name), age(age), email(email) {}

    // Virtual, because we delete derived objects through a Student* pointer.
    // Without this the derived destructor never runs.
    virtual ~Person() = default;

    const std::string& getName()  const { return name; }
    int                getAge()   const { return age; }
    const std::string& getEmail() const { return email; }

    void setName(const std::string& value)  { if (!value.empty()) name = value; }
    void setEmail(const std::string& value) { if (!value.empty()) email = value; }
    void setAge(int value)                  { if (value > 0 && value < 120) age = value; }

    virtual std::string displayInfo() const = 0;
};

// =========================================================================
// ENCAPSULATION
//
// studentId, course and marks are private. Nothing outside the class can
// reach in and change them. Marks can only get in through addMark(), which
// rejects anything outside 0-100, so a Student object can never hold an
// impossible mark. studentId has a getter but deliberately no setter - the
// ID is the identity of the record, changing it later would break lookups.
// =========================================================================
class Student : public Person {
private:
    std::string         studentId;
    std::string         course;
    std::vector<double> marks;

public:
    Student(const std::string& id, const std::string& name, int age,
            const std::string& email, const std::string& course)
        : Person(name, age, email), studentId(id), course(course) {}

    const std::string&         getStudentId() const { return studentId; }
    const std::string&         getCourse()    const { return course; }
    const std::vector<double>& getMarks()     const { return marks; }

    void setCourse(const std::string& value) { if (!value.empty()) course = value; }

    bool addMark(double mark) {
        if (mark < 0.0 || mark > 100.0) return false;
        marks.push_back(mark);
        return true;
    }

    double average() const {
        if (marks.empty()) return 0.0;
        double total = 0.0;
        for (double mark : marks) total += mark;
        return total / static_cast<double>(marks.size());
    }

    // Written once, here in the base class, but the result is different for
    // an undergraduate and a postgraduate because passMark() is virtual.
    bool hasPassed() const { return average() >= passMark(); }

    // The three things every kind of student must answer for itself.
    virtual std::string calculateGrade() const = 0;
    virtual std::string type()           const = 0;
    virtual double      passMark()       const = 0;

    std::string displayInfo() const override;
};

std::string Student::displayInfo() const {
    std::ostringstream row;
    row << std::left
        << std::setw(8)  << fit(studentId, 8)
        << std::setw(22) << fit(name, 22)
        << std::setw(5)  << age
        << std::setw(12) << fit(course, 12)
        << std::setw(16) << type()
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(8)  << average()
        << std::setw(7)  << calculateGrade()
        << std::setw(9)  << (hasPassed() ? "Pass" : "Fail");
    return row.str();
}

// =========================================================================
// POLYMORPHISM
//
// Both subclasses override calculateGrade(), type() and passMark(). The rest
// of the program never asks "which kind of student is this?" - it just calls
// the method on a Student pointer and the correct version runs.
// =========================================================================
class UndergraduateStudent : public Student {
public:
    using Student::Student;   // same constructor parameters as Student

    std::string calculateGrade() const override {
        double avg = average();
        if (avg >= 90) return "A+";
        if (avg >= 80) return "A";
        if (avg >= 70) return "B";
        if (avg >= 60) return "C";
        if (avg >= 50) return "D";
        return "F";
    }

    std::string type()     const override { return "Undergraduate"; }
    double      passMark() const override { return 50.0; }
};

class PostgraduateStudent : public Student {
public:
    using Student::Student;

    // Postgraduate marking is stricter - the same average earns a lower grade.
    std::string calculateGrade() const override {
        double avg = average();
        if (avg >= 90) return "A+";
        if (avg >= 85) return "A";
        if (avg >= 75) return "B";
        if (avg >= 65) return "C";
        if (avg >= 55) return "D";
        return "F";
    }

    std::string type()     const override { return "Postgraduate"; }
    double      passMark() const override { return 55.0; }
};

// =========================================================================
// The system class. It handles the collection of records; the Student
// objects handle themselves. Keeping those two jobs apart is what made the
// menu code short.
//
// We store unique_ptr<Student> because Student is abstract (a plain
// vector<Student> will not even compile) and because storing base objects by
// value slices off everything the subclass added.
// =========================================================================
class StudentManagementSystem {
private:
    std::vector<std::unique_ptr<Student>> students;

    static std::string separator() { return std::string(87, '-'); }

    static std::string tableHeader() {
        std::ostringstream head;
        head << std::left
             << std::setw(8)  << "ID"
             << std::setw(22) << "NAME"
             << std::setw(5)  << "AGE"
             << std::setw(12) << "COURSE"
             << std::setw(16) << "TYPE"
             << std::right
             << std::setw(8)  << "AVERAGE"
             << std::setw(7)  << "GRADE"
             << std::setw(9)  << "RESULT";
        return head.str();
    }

public:
    bool addStudent(std::unique_ptr<Student> student) {
        if (!student) return false;
        if (findStudent(student->getStudentId())) return false;   // duplicate ID
        students.push_back(std::move(student));
        return true;
    }

    // Returns a plain pointer on purpose - the caller borrows the object,
    // the vector keeps owning it. nullptr means "no such ID".
    Student* findStudent(const std::string& id) const {
        for (const auto& student : students)
            if (student->getStudentId() == id) return student.get();
        return nullptr;
    }

    bool removeStudent(const std::string& id) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if ((*it)->getStudentId() == id) {
                students.erase(it);
                return true;
            }
        }
        return false;
    }

    std::size_t count() const { return students.size(); }

    void displayAll() const {
        if (students.empty()) {
            std::cout << "\nThere are no student records yet.\n";
            return;
        }
        std::cout << "\n" << separator() << "\n"
                  << tableHeader() << "\n"
                  << separator() << "\n";
        for (const auto& student : students)
            std::cout << student->displayInfo() << "\n";   // polymorphic call
        std::cout << separator() << "\n"
                  << students.size() << " record(s) found.\n";
    }

    void displayReport() const {
        if (students.empty()) {
            std::cout << "\nNothing to report - no records yet.\n";
            return;
        }

        double total = 0.0;
        std::size_t passed = 0;
        const Student* topper = nullptr;

        for (const auto& student : students) {
            total += student->average();
            if (student->hasPassed()) ++passed;
            if (!topper || student->average() > topper->average())
                topper = student.get();
        }

        std::cout << "\n--- Class Report ---\n"
                  << std::fixed << std::setprecision(2)
                  << "Total students : " << students.size() << "\n"
                  << "Class average  : " << total / static_cast<double>(students.size()) << "\n"
                  << "Passed         : " << passed << " of " << students.size() << "\n"
                  << "Top scorer     : " << topper->getName()
                  << " (" << topper->getStudentId() << ") - "
                  << topper->average() << ", grade " << topper->calculateGrade() << "\n";
    }
};

// -------------------------------------------------------------------------
// Menu actions
// -------------------------------------------------------------------------

std::unique_ptr<Student> buildStudent() {
    std::cout << "\n--- Add Student ---\n";

    std::string id = input::readText("Student ID   : ");
    if (id.empty()) return nullptr;
    std::string name   = input::readText("Name         : ");
    int         age    = input::readInt ("Age          : ", 1, 119);
    std::string email  = input::readText("Email        : ");
    std::string course = input::readText("Course       : ");

    int choice = input::readInt("Type (1 = Undergraduate, 2 = Postgraduate) : ", 1, 2);

    // The only place in the program that decides which subclass to build.
    // Everything after this point just works with a Student pointer.
    std::unique_ptr<Student> student;
    if (choice == 2)
        student = std::make_unique<PostgraduateStudent>(id, name, age, email, course);
    else
        student = std::make_unique<UndergraduateStudent>(id, name, age, email, course);

    int howMany = input::readInt("How many marks to enter? ", 1, 20);
    for (int i = 1; i <= howMany; ++i)
        student->addMark(input::readDouble("  Mark " + std::to_string(i) + " : ", 0.0, 100.0));

    return student;
}

void searchStudent(const StudentManagementSystem& system) {
    std::string id = input::readText("\nEnter Student ID : ");
    if (id.empty()) return;

    Student* student = system.findStudent(id);
    if (!student) {
        std::cout << "No student with ID " << id << ".\n";
        return;
    }

    std::cout << "\n" << student->displayInfo() << "\n";
    std::cout << "Email : " << student->getEmail() << "\n";
    std::cout << "Marks : ";
    for (double mark : student->getMarks()) std::cout << mark << "  ";
    std::cout << "\n";
}

void updateStudent(StudentManagementSystem& system) {
    std::string id = input::readText("\nEnter Student ID to update : ");
    if (id.empty()) return;

    Student* student = system.findStudent(id);
    if (!student) {
        std::cout << "No student with ID " << id << ".\n";
        return;
    }

    std::cout << "Press Enter to keep the current value.\n";
    student->setName (input::readOptional("Name  [" + student->getName()   + "] : "));
    student->setEmail(input::readOptional("Email [" + student->getEmail()  + "] : "));
    student->setCourse(input::readOptional("Course [" + student->getCourse() + "] : "));

    std::string ageText = input::readOptional("Age   [" + std::to_string(student->getAge()) + "] : ");
    if (!ageText.empty()) {
        std::istringstream stream(ageText);
        int newAge = 0;
        if (stream >> newAge) student->setAge(newAge);
        else std::cout << "Age was not a number, keeping the old one.\n";
    }

    std::cout << "Record updated.\n";
}

void addMarks(StudentManagementSystem& system) {
    std::string id = input::readText("\nEnter Student ID : ");
    if (id.empty()) return;

    Student* student = system.findStudent(id);
    if (!student) {
        std::cout << "No student with ID " << id << ".\n";
        return;
    }

    int howMany = input::readInt("How many marks to add? ", 1, 20);
    for (int i = 1; i <= howMany; ++i)
        student->addMark(input::readDouble("  Mark " + std::to_string(i) + " : ", 0.0, 100.0));

    std::cout << "Marks added. New average is " << std::fixed << std::setprecision(2)
              << student->average() << " (grade " << student->calculateGrade() << ").\n";
}

void deleteStudent(StudentManagementSystem& system) {
    std::string id = input::readText("\nEnter Student ID to delete : ");
    if (id.empty()) return;

    if (system.removeStudent(id)) std::cout << "Record deleted.\n";
    else                          std::cout << "No student with ID " << id << ".\n";
}

// Two records so the menu has something to show the moment it starts.
// Delete this function if you want to begin with an empty system.
void seedDemoData(StudentManagementSystem& system) {
    auto aarav = std::make_unique<UndergraduateStudent>(
        "UG001", "Aarav Sharma", 19, "aarav@example.com", "BCA");
    aarav->addMark(85);
    aarav->addMark(78);
    aarav->addMark(92);

    auto meera = std::make_unique<PostgraduateStudent>(
        "PG001", "Meera Kapoor", 23, "meera@example.com", "MCA");
    meera->addMark(91);
    meera->addMark(88);
    meera->addMark(95);

    system.addStudent(std::move(aarav));
    system.addStudent(std::move(meera));
}

void showMenu() {
    std::cout << "\n=======================================\n"
              << "      STUDENT MANAGEMENT SYSTEM\n"
              << "=======================================\n"
              << " 1. Add Student\n"
              << " 2. View All Students\n"
              << " 3. Search Student by ID\n"
              << " 4. Update Student\n"
              << " 5. Add Marks\n"
              << " 6. Delete Student\n"
              << " 7. Class Report\n"
              << " 8. Exit\n"
              << "=======================================\n";
}

int main() {
    StudentManagementSystem system;
    seedDemoData(system);

    while (true) {
        showMenu();
        int choice = input::readInt("Enter your choice : ", 1, 8);

        if (!std::cin) {                      // Ctrl-D or end of piped input
            std::cout << "\nInput closed. Exiting.\n";
            break;
        }
        if (choice == 8) {
            std::cout << "\nThanks for using the Student Management System.\n";
            break;
        }

        switch (choice) {
            case 1: {
                auto student = buildStudent();
                if (!student) break;
                if (system.addStudent(std::move(student)))
                    std::cout << "Student added successfully.\n";
                else
                    std::cout << "That Student ID is already taken.\n";
                break;
            }
            case 2: system.displayAll();    break;
            case 3: searchStudent(system);  break;
            case 4: updateStudent(system);  break;
            case 5: addMarks(system);       break;
            case 6: deleteStudent(system);  break;
            case 7: system.displayReport(); break;
        }
    }

    return 0;
}
