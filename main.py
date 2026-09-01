from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import List, Optional


class Person(ABC):
    """Base class representing a person."""

    def __init__(self, name: str, age: int, email: str):
        self.name = name
        self.age = age
        self.email = email

    @abstractmethod
    def display_info(self) -> str:
        pass


class Student(Person):
    """Base Student class demonstrating encapsulation and inheritance."""

    def __init__(self, student_id: str, name: str, age: int, email: str, course: str):
        super().__init__(name, age, email)
        self.__student_id = student_id
        self.__marks: List[float] = []
        self.course = course

    @property
    def student_id(self) -> str:
        return self.__student_id

    @property
    def marks(self) -> List[float]:
        return self.__marks.copy()

    def update_details(self, name=None, age=None, email=None, course=None):
        if name:
            self.name = name
        if age is not None:
            self.age = age
        if email:
            self.email = email
        if course:
            self.course = course

    def add_marks(self, marks: List[float]):
        if not marks:
            raise ValueError("At least one mark is required.")
        if any(mark < 0 or mark > 100 for mark in marks):
            raise ValueError("Marks must be between 0 and 100.")
        self.__marks.extend(marks)

    def average(self) -> float:
        return sum(self.__marks) / len(self.__marks) if self.__marks else 0.0

    @abstractmethod
    def calculate_grade(self) -> str:
        pass

    def display_info(self) -> str:
        return (
            f"ID: {self.student_id} | Name: {self.name} | Age: {self.age} | "
            f"Email: {self.email} | Course: {self.course} | "
            f"Average: {self.average():.2f} | Grade: {self.calculate_grade()}"
        )


class UndergraduateStudent(Student):
    """Undergraduate student with standard grading."""

    def calculate_grade(self) -> str:
        avg = self.average()
        if avg >= 90:
            return "A+"
        if avg >= 80:
            return "A"
        if avg >= 70:
            return "B"
        if avg >= 60:
            return "C"
        if avg >= 50:
            return "D"
        return "F"


class PostgraduateStudent(Student):
    """Postgraduate student with a slightly stricter grading scale."""

    def calculate_grade(self) -> str:
        avg = self.average()
        if avg >= 90:
            return "A+"
        if avg >= 85:
            return "A"
        if avg >= 75:
            return "B"
        if avg >= 65:
            return "C"
        if avg >= 55:
            return "D"
        return "F"


class StudentManagementSystem:
    """Manages Student objects using an object-oriented design."""

    def __init__(self):
        self.__students: List[Student] = []

    def add_student(self, student: Student):
        if self.find_student(student.student_id):
            raise ValueError("A student with this ID already exists.")
        self.__students.append(student)

    def find_student(self, student_id: str) -> Optional[Student]:
        return next((s for s in self.__students if s.student_id == student_id), None)

    def update_student(self, student_id: str, **details):
        student = self.find_student(student_id)
        if not student:
            raise ValueError("Student not found.")
        student.update_details(**details)

    def delete_student(self, student_id: str):
        student = self.find_student(student_id)
        if not student:
            raise ValueError("Student not found.")
        self.__students.remove(student)

    def get_all_students(self) -> List[Student]:
        return self.__students.copy()

    def display_all(self):
        students = self.get_all_students()
        if not students:
            print("\nNo student records found.")
            return
        print("\n" + "=" * 105)
        print("STUDENT RECORDS")
        print("=" * 105)
        for student in students:
            print(student.display_info())
        print("=" * 105)


def get_int(prompt: str, minimum=None, maximum=None) -> int:
    while True:
        try:
            value = int(input(prompt))
            if minimum is not None and value < minimum:
                raise ValueError
            if maximum is not None and value > maximum:
                raise ValueError
            return value
        except ValueError:
            limits = ""
            if minimum is not None and maximum is not None:
                limits = f" ({minimum}-{maximum})"
            print(f"Invalid input. Enter a valid integer{limits}.")


def get_float(prompt: str, minimum=0, maximum=100) -> float:
    while True:
        try:
            value = float(input(prompt))
            if minimum <= value <= maximum:
                return value
        except ValueError:
            pass
        print(f"Invalid input. Enter a number between {minimum} and {maximum}.")


def create_student() -> Student:
    print("\n--- Add Student ---")
    student_id = input("Student ID: ").strip()
    name = input("Name: ").strip()
    age = get_int("Age: ", 1, 100)
    email = input("Email: ").strip()
    course = input("Course: ").strip()

    print("Student type:")
    print("1. Undergraduate")
    print("2. Postgraduate")
    student_type = input("Choose type: ").strip()

    if student_type == "2":
        student = PostgraduateStudent(student_id, name, age, email, course)
    else:
        student = UndergraduateStudent(student_id, name, age, email, course)

    count = get_int("Number of marks to add: ", 1, 20)
    marks = [get_float(f"Mark {i + 1}: ") for i in range(count)]
    student.add_marks(marks)
    return student


def update_student(system: StudentManagementSystem):
    print("\n--- Update Student ---")
    student_id = input("Enter Student ID: ").strip()
    student = system.find_student(student_id)
    if not student:
        print("Student not found.")
        return

    print("Leave a field blank to keep its current value.")
    name = input(f"Name [{student.name}]: ").strip() or None
    age_text = input(f"Age [{student.age}]: ").strip()
    age = int(age_text) if age_text else None
    email = input(f"Email [{student.email}]: ").strip() or None
    course = input(f"Course [{student.course}]: ").strip() or None

    system.update_student(student_id, name=name, age=age, email=email, course=course)
    print("Student updated successfully.")


def search_student(system: StudentManagementSystem):
    student_id = input("\nEnter Student ID: ").strip()
    student = system.find_student(student_id)
    if student:
        print("\n" + student.display_info())
        print(f"Marks: {student.marks}")
    else:
        print("Student not found.")


def add_marks(system: StudentManagementSystem):
    student_id = input("\nEnter Student ID: ").strip()
    student = system.find_student(student_id)
    if not student:
        print("Student not found.")
        return
    count = get_int("Number of marks to add: ", 1, 20)
    marks = [get_float(f"Mark {i + 1}: ") for i in range(count)]
    student.add_marks(marks)
    print("Marks added successfully.")


def seed_demo_data(system: StudentManagementSystem):
    """Add sample records so the project is immediately demonstrable."""
    a = UndergraduateStudent("UG001", "Aarav Sharma", 19, "aarav@example.com", "BCA")
    a.add_marks([85, 78, 92])
    b = PostgraduateStudent("PG001", "Meera Kapoor", 23, "meera@example.com", "MCA")
    b.add_marks([91, 88, 95])
    system.add_student(a)
    system.add_student(b)


def main():
    system = StudentManagementSystem()
    seed_demo_data(system)

    while True:
        print("\n" + "=" * 55)
        print("       STUDENT MANAGEMENT SYSTEM")
        print("=" * 55)
        print("1. Add Student")
        print("2. View All Students")
        print("3. Search Student")
        print("4. Update Student")
        print("5. Add/Update Marks")
        print("6. Delete Student")
        print("7. Exit")
        print("=" * 55)

        choice = input("Enter your choice: ").strip()
        try:
            if choice == "1":
                student = create_student()
                system.add_student(student)
                print("Student added successfully.")
            elif choice == "2":
                system.display_all()
            elif choice == "3":
                search_student(system)
            elif choice == "4":
                update_student(system)
            elif choice == "5":
                add_marks(system)
            elif choice == "6":
                student_id = input("Enter Student ID to delete: ").strip()
                system.delete_student(student_id)
                print("Student deleted successfully.")
            elif choice == "7":
                print("Thank you for using Student Management System!")
                break
            else:
                print("Invalid choice. Please select 1-7.")
        except ValueError as error:
            print(f"Error: {error}")
        except (EOFError, KeyboardInterrupt):
            print("\nProgram terminated.")
            break


if __name__ == "__main__":
    main()
