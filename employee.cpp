#include "pch.h"
#include <string>
#include <iostream>
#include "employee.h"

Employee::Employee(std::string name, int age, double salary) {
    this->name = name;
    setAge(age);
    setSalary(salary);
    this->retired = false;
    this->hired = false;
    this->manager = nullptr;
}

void Employee::setName(std::string name) 
{ 
    this->name = name; 
}

void Employee::setAge(int age) 
{
    if (age <= 0)
    {
        throw "Illegal Argument";
    }
    this->age = age; 
}

void Employee::setSalary(double salary) 
{ 
    if (salary < 0)
    {
        throw "Illegal Argument";
    }
    this->salary = salary; 
}

std::string Employee::getName() { return this->name; }

int Employee::getAge() { return this->age; }

double Employee::getSalary() { return this->salary; }

void Employee::print() {
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Age: " << this->age << std::endl;
    std::cout << "Salary: " << this->salary << std::endl;
    std::cout << "Manager: "
        << ((this->manager != nullptr) ? this->manager->getName() : "None")
        << std::endl;
    std::cout << "Status: "
        << ((this->retired) ? "Retired"
            : ((this->hired) ? "Hired" : "Unemployed"))
        << std::endl;
}

void Employee::promote(double amount) { this->salary += amount; }

void Employee::demote(double amount) { this->salary -= amount; }

void Employee::retire() {
    this->retired = true;
    this->hired = false;
}

void Employee::hire() {
    this->hired = true;
    this->retired = false;
}

bool Employee::isRetired() { return this->retired; }

bool Employee::isHired() { return this->hired; }

void Employee::setManager(Employee* manager) { this->manager = manager; }

Employee* Employee::getManager() { return this->manager; }
