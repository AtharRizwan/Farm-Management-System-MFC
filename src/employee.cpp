#include "pch.h"
#include <cmath>
#include <stdexcept>
#include <string>
#include "employee.h"

Employee::Employee(const std::string& name, int age, double salary) {
    setName(name);
    setAge(age);
    setSalary(salary);
}

void Employee::setName(const std::string& name)
{
    size_t first = name.find_first_not_of(" \t");
    if (first == std::string::npos)
    {
        throw std::invalid_argument("Name cannot be empty");
    }
    this->name = name.substr(first, name.find_last_not_of(" \t") - first + 1);
}

void Employee::setAge(int age)
{
    if (age < 14 || age > 100)
    {
        throw std::invalid_argument("Age must be between 14 and 100");
    }
    this->age = age;
}

void Employee::setSalary(double salary)
{
    if (salary < 0 || !std::isfinite(salary))
    {
        throw std::invalid_argument("Salary cannot be negative");
    }
    this->salary = salary;
}

const std::string& Employee::getName() const { return name; }

int Employee::getAge() const { return age; }

double Employee::getSalary() const { return salary; }

void Employee::promote(double amount) { setSalary(salary + amount); }

void Employee::demote(double amount) { setSalary(salary - amount); }
