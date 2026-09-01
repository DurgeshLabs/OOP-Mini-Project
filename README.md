# Student Management System

A console-based Student Management System written in C++ for our OOP mini project.
It stores student records, updates them, calculates averages and grades, and shows
the difference between doing all of this procedurally and doing it with classes.

The repository contains two programs that solve the same problem:

- `src/main.cpp` — the object-oriented version, which is the actual project
- `src/procedural_version.cpp` — the same records handled with global arrays and
  functions, kept so the two approaches can be compared directly

---

## Features

- Add student records (undergraduate or postgraduate)
- View all records in a formatted table
- Search a student by ID
- Update name, email, course or age
- Add marks to an existing student
- Delete a record
- Class report: class average, pass count and top scorer
- Marks validated to 0–100, duplicate IDs rejected, bad input re-prompted
- Two grading scales, selected automatically by the type of student

---

## Requirements

- A C++17 compiler (`g++` or `clang++`)
- `make` (optional — the direct compile command is given below)

No libraries beyond the standard library.

---

## How to build and run

```bash
git clone https://github.com/DurgeshLabs/OOP-Mini-Project.git
cd OOP-Mini-Project
make
./bin/sms
```

Without `make`:

```bash
g++ -std=c++17 -Wall -Wextra -o sms src/main.cpp
./sms
```

To run the procedural version for comparison:

```bash
make compare
```

Both files compile clean with `-Wall -Wextra`.

---

## What it looks like

```text
=======================================
      STUDENT MANAGEMENT SYSTEM
=======================================
 1. Add Student
 2. View All Students
 3. Search Student by ID
 4. Update Student
 5. Add Marks
 6. Delete Student
 7. Class Report
 8. Exit
=======================================
Enter your choice : 2

---------------------------------------------------------------------------------------
ID      NAME                  AGE  COURSE      TYPE             AVERAGE  GRADE   RESULT
---------------------------------------------------------------------------------------
UG001   Aarav Sharma          19   BCA         Undergraduate      85.00      A     Pass
PG001   Meera Kapoor          23   MCA         Postgraduate       91.33     A+     Pass
---------------------------------------------------------------------------------------
2 record(s) found.
```

The program starts with those two demo records so there is something to look at
immediately. Removing the `seedDemoData()` call in `main()` starts it empty.

---

## How we built it

We did not start with the class diagram. We started with the procedural version
because that is what we already knew, and moved to classes only when the
procedural code started fighting us. Going in that order is the reason the OOP
version looks the way it does.

**Step 1 — get it working procedurally.**
Seven parallel arrays (`ids[]`, `names[]`, `ages[]`, `marks[][]`, and so on) and a
set of functions taking an index. It worked. It is still in the repo as
`src/procedural_version.cpp`. Two problems showed up almost immediately:

- Index 3 of every array had to describe the same student and nothing enforced
  that. Deleting a record properly meant shifting seven arrays.
- `addStudent()` takes six parameters, five of them strings. Swapping email and
  course compiles perfectly and produces a wrong record.

**Step 2 — group the data.**
The first real change was pulling the scattered arrays into one `Student` class so
that one variable holds one student. This is where marks stopped being
`marks[i][j]` and became a `std::vector<double>` that belongs to the object.

**Step 3 — make the data private.**
Once marks lived inside the class we made them `private` and added `addMark()`,
which rejects anything outside 0–100. That single change means no code anywhere
in the program can put an invalid mark into a `Student`. The check exists in one
place instead of at every call site.

**Step 4 — split undergraduate and postgraduate.**
The requirement was that postgraduate marking is stricter. Procedurally this was
`if (types[i] == "PG") { ... } else { ... }` inside `calculateGrade()`, plus a
second copy of the same branch inside `hasPassed()`. Two branches on the same
condition in two functions was the point where inheritance obviously won. We
pulled everything common into `Student` and gave `UndergraduateStudent` and
`PostgraduateStudent` their own `calculateGrade()` and `passMark()`.

**Step 5 — separate the record from the system.**
`StudentManagementSystem` ended up owning the collection and nothing else. A
`Student` knows how to compute its own average and grade; the system knows how to
add, find and remove. Once that split was clear the menu code became short,
because it mostly just forwards to one of those two.

**Step 6 — fix the input handling.**
Covered in the "what went wrong" section below. It took longer than the classes did.

---

## The OOP concepts, in our code

### 1. Abstraction

`Person` defines what every person must be able to do without saying how:

```cpp
class Person {
protected:
    std::string name;
    int         age;
    std::string email;
public:
    virtual std::string displayInfo() const = 0;   // pure virtual
};
```

`Student` adds three more pure virtual functions, so it is abstract too:

```cpp
virtual std::string calculateGrade() const = 0;
virtual std::string type()           const = 0;
virtual double      passMark()       const = 0;
```

You cannot create a plain `Student`, and that is correct — "a student" with no
level does not have a grading rule. Only `UndergraduateStudent` and
`PostgraduateStudent` can be instantiated. The compiler enforces the design.

### 2. Encapsulation

```cpp
class Student : public Person {
private:
    std::string         studentId;
    std::string         course;
    std::vector<double> marks;
public:
    bool addMark(double mark) {
        if (mark < 0.0 || mark > 100.0) return false;
        marks.push_back(mark);
        return true;
    }
};
```

`marks` is private, so `addMark()` is the only way in and the range check cannot
be bypassed. Compare that with the procedural file, where any line of code can
write `marks[i][j] = 250;` and skip the validation entirely.

`studentId` has a getter but deliberately no setter. The ID is the identity of the
record — every lookup uses it — so allowing it to change later would silently
break `findStudent()`. Leaving the setter out was a decision, not an oversight.

### 3. Inheritance

```cpp
class Student : public Person { ... };
class UndergraduateStudent : public Student { ... };
class PostgraduateStudent  : public Student { ... };
```

`average()`, `hasPassed()`, `displayInfo()`, the marks vector and all the getters
are written once in `Student`. The two subclasses only contain what is genuinely
different between them — their grading scale and their pass mark. Each subclass is
about fifteen lines.

### 4. Polymorphism

This is the part that made the idea click for us. `displayAll()` contains no test
for the type of student:

```cpp
for (const auto& student : students)
    std::cout << student->displayInfo() << "\n";
```

`displayInfo()` calls `calculateGrade()`, and the right override runs based on
what the object actually is. Same loop, same call, different behaviour. Two
students with an identical average get different grades:

```text
X1      UG Eighty Two         20   BCA         Undergraduate      82.00      A     Pass
X2      PG Eighty Two         24   MCA         Postgraduate       82.00      B     Pass
```

`hasPassed()` is worth pointing out separately. It is written **once**, in the base
class, and it is not virtual:

```cpp
bool hasPassed() const { return average() >= passMark(); }
```

but `passMark()` is virtual, so this one function gives the right answer for both
subclasses. Shared logic in the base, only the varying part overridden.

---

## Procedural vs object-oriented

| Procedural version | Object-oriented version |
|---|---|
| Seven parallel arrays that must stay in sync manually | One object holds one student |
| Data is global; any function can modify it | Data is private; access goes through methods |
| Validation can be skipped by writing to the array directly | `addMark()` is the only way in, so the rule always applies |
| Grading rules chosen with `if (types[i] == "PG")` | The object already knows its own rule |
| A new student type means editing several functions | A new student type is a new class |
| Deleting a record means shifting seven arrays | `students.erase(it)` |
| Six positional parameters, easy to swap by mistake | Constructor bound to one specific type |

The concrete test we used: **what does it take to add a diploma student with its
own grading scale?**

- Procedural: edit `calculateGrade()`, edit `hasPassed()`, check every other place
  that compares the type string, and hope none were missed.
- OOP: add one class with two overridden methods and one line in the menu. Nothing
  that already worked has to be touched.

That difference is the whole argument for OOP, and it is much more convincing
after writing both versions than after reading about it.

---

## What went wrong along the way

These cost us the most time, so they are worth writing down.

**`cin >>` followed by `getline` returned an empty string.**
Reading the age with `cin >> age` leaves the newline in the buffer, and the next
`getline` immediately consumes it and returns nothing. We tried `cin.ignore()` and
kept getting it subtly wrong, so in the end every input goes through `getline` and
is parsed with a `std::istringstream`. That is what the `input` namespace at the
top of `main.cpp` is for. It also made range checking easy to apply in one place.

**`std::vector<Student>` would not compile.**
Because `Student` is abstract, it cannot be stored by value. Our first workaround
was to store `Person` objects by value, which compiled but produced garbage — the
derived parts were being sliced off. Object slicing was completely new to us. The
fix was `std::vector<std::unique_ptr<Student>>`: the vector holds pointers, so the
objects stay polymorphic and are still cleaned up automatically.

**Missing virtual destructor.**
Once we were deleting derived objects through a base pointer, the base destructor
needed to be `virtual`. Without it the derived destructor never runs. Nothing
visibly broke in this small program, which is exactly what makes it dangerous.

**Dangling pointers after erase.**
`findStudent()` returns a raw `Student*` that borrows the object while the vector
keeps owning it. That pointer becomes invalid after the record is deleted. In this
program each menu action finishes before anything is deleted, so it is safe, but
this is the kind of thing a longer-lived program would have to handle properly.

---

## What we understood from this project

- A class is a way of putting data and the rules about that data in the same
  place. In the procedural file the rule "marks are between 0 and 100" was written
  in a function that anybody could bypass; in the OOP file it is a property of the
  type itself.
- Private members are not about secrecy, they are about reducing how many places
  in the program can break an invariant. Fewer entry points means fewer bugs.
- Inheritance is about reusing behaviour, but its real value showed up when we
  wanted to *change* behaviour for one type without touching the others.
- Polymorphism removes conditionals. Every `if (type == "PG")` we deleted became a
  method on a class, and the calling code stopped caring about the type at all.
- Abstract classes let the compiler enforce the design. Making `Student` abstract
  means a half-defined student is impossible to create.
- Ownership matters in C++ in a way it does not in a language with a garbage
  collector. Choosing `unique_ptr` and returning borrowed raw pointers was a
  deliberate decision, not a detail.
- Splitting `Student` from `StudentManagementSystem` was the change that made the
  menu code readable. Giving each class exactly one job is worth more than any
  individual OOP keyword.

---

## Project structure

```text
OOP-Mini-Project/
├── src/
│   ├── main.cpp                 # object-oriented version (the project)
│   └── procedural_version.cpp   # procedural version, for comparison
├── Makefile                     # build both programs into bin/
├── .gitignore
├── LICENSE
└── README.md
```

---

## Possible improvements

- Save records to a file so they survive between runs
- Split the classes into separate `.h` / `.cpp` files
- Unit tests for `average()` and the two grading scales
- Sort and filter the table by course or grade
- A diploma student class, to demonstrate how little has to change

---

## Authors

**Yash (Main)** · **Arnav** · **Adarsh** · **Durgesh**

---

## License

MIT License — see [LICENSE](LICENSE) for details.
