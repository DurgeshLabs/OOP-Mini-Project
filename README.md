# 🎓 Student Management System

A **Student Management System** designed to demonstrate the fundamental concepts of **Object-Oriented Programming (OOP)**. The application allows users to create, update, and view student records while implementing important OOP principles such as **encapsulation, inheritance, and polymorphism**.

## 🚀 Features

* ➕ Add new student records
* ✏️ Update existing student information
* 👀 View student details
* 📋 Display all student records
* 🔒 Protect data using encapsulation
* 🧬 Demonstrate inheritance using specialized student classes
* 🔄 Implement polymorphism through overridden methods
* ⚖️ Compare procedural and object-oriented programming approaches

---

## 🧠 OOP Concepts Demonstrated

### 1. Encapsulation

Student attributes are kept private and accessed or modified through controlled methods such as getters and setters.

```text
Student
├── Private Attributes
│   ├── ID
│   ├── Name
│   └── Marks
└── Public Methods
    ├── Getters
    ├── Setters
    └── Display Information
```

### 2. Inheritance

Specialized student classes inherit common properties and behaviour from a base `Student` class.

```text
          Student
             │
      ┌──────┴──────┐
      │             │
RegularStudent  SpecialStudent
```

This prevents code duplication and promotes code reusability.

### 3. Polymorphism

Different student types can provide their own implementation of common methods, such as displaying student information or calculating grades.

---

## 🏗️ Class Structure

```text
Student Management System
│
├── Student
│   ├── studentId
│   ├── name
│   ├── age
│   └── marks
│
├── RegularStudent
│   └── Inherits Student
│
├── SpecialStudent
│   └── Inherits Student
│
└── StudentManagementSystem
    ├── addStudent()
    ├── updateStudent()
    ├── viewStudent()
    └── displayAllStudents()
```

---

## ⚖️ Procedural vs Object-Oriented Approach

| Procedural Programming          | Object-Oriented Programming            |
| ------------------------------- | -------------------------------------- |
| Focuses on functions            | Focuses on objects                     |
| Data and functions are separate | Data and behaviour are combined        |
| Less suitable for large systems | Better scalability and maintainability |
| Code reuse is limited           | Supports inheritance and polymorphism  |
| Data security is limited        | Supports encapsulation                 |

This project demonstrates how a procedural approach to managing student records can be transformed into an **object-oriented design** using classes and objects.

---

## 🛠️ Core Functionalities

### ➕ Create Student

A new student record can be created by providing details such as:

* Student ID
* Name
* Age
* Marks

### ✏️ Update Student

Existing student information can be modified based on the student's unique ID.

### 👀 View Student

The complete details of a specific student can be displayed.

### 📋 View All Students

The system can display all available student records.

---

## 💻 Sample Output

```text
====== STUDENT MANAGEMENT SYSTEM ======

1. Add Student
2. Update Student
3. View Student
4. View All Students
5. Exit

Enter your choice: 1

Enter Student ID: 101
Enter Name: Yash
Enter Age: 19
Enter Marks: 92

Student added successfully!
```

---

## 📚 Learning Outcomes

After completing this project, you should understand:

* How to create and use classes and objects
* The importance of encapsulation
* How inheritance promotes code reuse
* How polymorphism allows flexible behaviour
* The difference between procedural and object-oriented programming
* How OOP concepts can be applied to a real-world management system

---

## 🧰 Technologies Used

* Programming Language: **C++**
* Concepts: **Object-Oriented Programming**
* Tools: **Git and GitHub**

---

## 📁 Project Structure

```text
Student-Management-System/
│
├── src/
│   └── main.cpp
│
├── README.md
│
└── .gitignore
```

---

## 🎯 Objective

The objective of this project is to provide a practical implementation of fundamental **Object-Oriented Programming concepts** by developing a simple and structured Student Management System.

The project emphasizes writing **modular, reusable, maintainable, and scalable code** while demonstrating the advantages of object-oriented programming over traditional procedural programming.

---

## 👨‍💻 Author

**Yash(Main)** **Arnav** **Adarsh** **Durgesh**

---

⭐ If you found this project useful, consider giving the repository a star!
