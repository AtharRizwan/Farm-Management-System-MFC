#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class Employee {
private:
    std::string name;
    int age = 0;
    double salary = 0;

public:
    // Throws std::invalid_argument if a value is not allowed
    Employee(const std::string& name, int age, double salary);

    void setName(const std::string& name);
    void setAge(int age);
    void setSalary(double salary);
    const std::string& getName() const;
    int getAge() const;
    double getSalary() const;
    void promote(double amount);
    void demote(double amount);
};

#endif // EMPLOYEE_H
