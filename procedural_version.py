"""Simple procedural version used to demonstrate the difference from OOP."""

students = []


def add_student(student_id, name, age, email, course, marks):
    students.append({
        "id": student_id,
        "name": name,
        "age": age,
        "email": email,
        "course": course,
        "marks": marks,
    })


def calculate_average(marks):
    return sum(marks) / len(marks) if marks else 0


def find_student(student_id):
    for student in students:
        if student["id"] == student_id:
            return student
    return None


def update_student(student_id, name=None, course=None):
    student = find_student(student_id)
    if student:
        if name:
            student["name"] = name
        if course:
            student["course"] = course


def display_students():
    for student in students:
        print(
            student["id"],
            student["name"],
            student["course"],
            f"Average: {calculate_average(student['marks']):.2f}",
        )


if __name__ == "__main__":
    add_student("P001", "Rahul", 20, "rahul@example.com", "BCA", [80, 85, 90])
    add_student("P002", "Priya", 21, "BCA", "BCA", [75, 88, 82])
    update_student("P001", name="Rahul Verma")
    display_students()
