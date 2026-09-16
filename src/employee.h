#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class Employee {
private:
    std::string name;
    int age;
    double salary;
    bool retired;
    bool hired;
    Employee* manager;

public:
    Employee(std::string name, int age, double salary);

    void setName(std::string name);
    void setAge(int age);
    void setSalary(double salary);
    std::string getName();
    int getAge();
    double getSalary();
    void print();
    void promote(double amount);
    void demote(double amount);
    void retire();
    void hire();
    bool isRetired();
    bool isHired();
    void setManager(Employee* manager);
    Employee* getManager();
};

#endif // EMPLOYEE_H

