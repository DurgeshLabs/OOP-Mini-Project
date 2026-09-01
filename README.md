# Student Management System

A Python console application designed to demonstrate the fundamental concepts of **Object-Oriented Programming (OOP)** through a practical Student Management System.

## Features

- Create student records
- View all student records
- Search students by ID
- Update student details
- Add/update marks
- Delete student records
- Calculate average marks
- Calculate grades
- Undergraduate and postgraduate student types
- Input validation and error handling
- Demonstrates encapsulation, inheritance, polymorphism and abstraction
- Includes a procedural implementation for comparison

## Project Structure

```text
student-management-system/
├── main.py                 # Complete OOP implementation
├── procedural_version.py   # Procedural approach for comparison
├── README.md               # Project documentation
├── requirements.txt        # Dependencies
├── .gitignore              # Git ignore rules
└── LICENSE                 # MIT License
```

## Requirements

- Python 3.9 or later
- No external packages are required

## How to Run

Clone the repository:

```bash
git clone https://github.com/YOUR-USERNAME/student-management-system.git
cd student-management-system
```

Run the application:

```bash
python main.py
```

On some systems:

```bash
python3 main.py
```

## OOP Concepts Demonstrated

### 1. Encapsulation

Student ID and marks are stored using private attributes:

```python
self.__student_id = student_id
self.__marks = []
```

Controlled access is provided through properties and methods such as `student_id`, `marks`, `add_marks()` and `update_details()`.

### 2. Inheritance

`Student` inherits common properties and behavior from `Person`:

```python
class Student(Person):
```

`UndergraduateStudent` and `PostgraduateStudent` then inherit from `Student`:

```python
class UndergraduateStudent(Student):
class PostgraduateStudent(Student):
```

### 3. Polymorphism

Both student subclasses implement the same method:

```python
def calculate_grade(self):
```

The grading behavior differs depending on the object type. The management system can work with either type through the common `Student` interface.

### 4. Abstraction

`Person` and `Student` use abstract methods to define behavior that subclasses must implement:

```python
@abstractmethod
def calculate_grade(self):
```

This keeps the design focused on what an object must do rather than how every implementation works internally.

## Procedural vs Object-Oriented Approach

The repository includes `procedural_version.py` so the difference can be demonstrated directly.

| Procedural Programming | Object-Oriented Programming |
|---|---|
| Data is stored in dictionaries/lists | Data is represented by objects |
| Functions operate on shared data | Objects combine data and behavior |
| Less natural data protection | Encapsulation protects internal state |
| Reuse relies mainly on functions | Reuse is supported by inheritance and composition |
| New student types require more conditional logic | New types can inherit and override behavior |
| Suitable for small/simple scripts | Better suited to larger systems with related entities |

## Example OOP Flow

```text
Person
  ↓
Student
  ↓
 ┌───────────────────────┐
 │                       │
Undergraduate       Postgraduate
Student             Student
```

A `StudentManagementSystem` object manages all student objects. This separates **student behavior** from **system-level operations**.

## Sample Records

The application starts with two demo records:

- UG001 — Aarav Sharma — BCA
- PG001 — Meera Kapoor — MCA

You can remove or modify the `seed_demo_data()` function if an empty database is preferred.

## Future Improvements

Possible extensions include:

- Save records to JSON/CSV/SQLite
- GUI using Tkinter
- REST API using Flask/FastAPI
- Authentication for administrators
- Attendance management
- Course and subject management
- Database integration
- Unit testing with pytest

## Academic Objective

This project demonstrates how OOP can model a real-world problem by representing people and students as objects. It specifically showcases **encapsulation, inheritance, polymorphism and abstraction**, while contrasting the design with a procedural implementation.

## License

MIT License — see `LICENSE` for details.
