class Student:
    def __init__(self, student_id, first_name, last_name, year_of_birth, course):
        self.student_id = student_id
        self.first_name = first_name
        self.last_name = last_name
        self.year_of_birth = year_of_birth
        self.course = course

    def __str__(self):
        return f"{self.student_id},{self.first_name},{self.last_name},{self.year_of_birth},{self.course}"

stu = Student('N1264601','Martin','Ofori','19/04/2004','Software Engineering')

print(stu.course)

