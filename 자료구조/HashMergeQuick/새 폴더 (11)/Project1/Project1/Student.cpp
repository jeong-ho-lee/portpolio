#include "Student.h"

void Print(ostream& out, Student stu[], int numelement)
{
	for(int i=0; i<numelement; i++)
	{
		stu[i].Print(out);
	}
}
void Student::Print(ostream& out)
{
	out << id << "\t" << name << "\t" << gpa << endl;
}
void Student::InitValue(int _id, char* _name, float _gpa)
{
	id = _id;
	strcpy_s(name,_name);
	gpa = _gpa;
}
void Student::getValue(int& _id, char* _name, float& _gpa)
{
	_id = id;
	strcpy_s(_name, sizeof(name), name);
	_gpa = gpa;	
}
char* Student::getName()
{
	return name;
}
void Student::operator = (Student stu)
{
	id = stu.id;
	strcpy_s(name,stu.name);
	gpa = stu.gpa;
}

bool Student::operator == (Student stu) {
	return (strcmp(name, stu.name) == 0);
}

bool Student::operator != (Student stu) {
	return !(strcmp(name, stu.name) == 0);
}

void PrintByPointer(ostream& out, Student* values[], int numValues)
{
	for(int i=0; i<numValues; i++)
	{
		(*values[i]).Print(out);
	}
}

char* Student::getKey() {
	return name;
}

Student Student::EmptyKey() {
	Student emptyStu;

	emptyStu.id = 0000000000;
	strcpy_s(emptyStu.name, sizeof(emptyStu.name), "None");
	emptyStu.gpa = 0.0;

	return emptyStu;
}