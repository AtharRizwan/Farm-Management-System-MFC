#include "pch.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "FarmData.h"

using namespace std;

namespace
{
	const char* const kEmployeeFile = "data/Employee.txt";

	// Data files that start from a copy in defaults/
	const char* const kDataFiles[] = { "Wheat.txt", "Corn.txt", "Rice.txt", "Harvest.txt", "Employee.txt" };

	// Copies each default file into data/ unless the user already has one.
	// A file that cannot be copied is reported by the load that follows.
	void ensureDataFiles()
	{
		CreateDirectoryA("data", nullptr);
		for (const char* name : kDataFiles)
		{
			string from = string("defaults\\") + name;
			string to = string("data\\") + name;
			CopyFileA(from.c_str(), to.c_str(), TRUE);
		}
	}

	// One employee per line: "name age salary". The name may contain spaces,
	// so age and salary are taken from the end of the line.
	Employee parseEmployee(const string& text, int line)
	{
		istringstream in(text);
		vector<string> tokens;
		string token;
		while (in >> token)
		{
			tokens.push_back(token);
		}
		string where = string(kEmployeeFile) + ": line " + to_string(line) + ": ";
		if (tokens.size() < 3)
		{
			throw runtime_error(where + "expected \"name age salary\"");
		}
		string name = tokens[0];
		for (size_t i = 1; i + 2 < tokens.size(); ++i)
		{
			name += " " + tokens[i];
		}
		try
		{
			size_t usedAge = 0;
			size_t usedSalary = 0;
			const string& ageText = tokens[tokens.size() - 2];
			const string& salaryText = tokens.back();
			int age = stoi(ageText, &usedAge);
			double salary = stod(salaryText, &usedSalary);
			if (usedAge != ageText.size() || usedSalary != salaryText.size())
			{
				throw runtime_error(where + "age and salary must be numbers");
			}
			return Employee(name, age, salary);
		}
		catch (const invalid_argument& e)
		{
			// Also thrown by stoi/stod for text that is not a number
			throw runtime_error(where + e.what());
		}
		catch (const out_of_range&)
		{
			throw runtime_error(where + "number is out of range");
		}
	}

	vector<Employee> loadEmployees()
	{
		ifstream in(kEmployeeFile);
		if (!in)
		{
			throw runtime_error(string(kEmployeeFile) + ": cannot open file for reading");
		}
		vector<Employee> result;
		string text;
		int line = 0;
		while (getline(in, text))
		{
			++line;
			if (text.find_first_not_of(" \t\r") == string::npos)
			{
				continue;
			}
			result.push_back(parseEmployee(text, line));
		}
		return result;
	}
}

void FarmData::loadAll()
{
	ensureDataFiles();

	Wheat newWheat;
	Corn newCorn;
	Rice newRice;
	Harvest newHarvest;
	newWheat.load();
	newCorn.load();
	newRice.load();
	newHarvest.load();
	vector<Employee> newEmployees = loadEmployees();

	wheat = newWheat;
	corn = newCorn;
	rice = newRice;
	harvest = newHarvest;
	employees = move(newEmployees);
}

void FarmData::saveCrops() const
{
	wheat.save();
	corn.save();
	rice.save();
	harvest.save();
}

void FarmData::saveEmployees() const
{
	const string path = kEmployeeFile;
	const string temp = path + ".tmp";
	{
		ofstream out(temp, ios::trunc);
		if (!out)
		{
			throw runtime_error(path + ": cannot open file for writing");
		}
		out.setf(ios::fixed);
		out.precision(2);
		for (const Employee& employee : employees)
		{
			out << employee.getName() << ' ' << employee.getAge() << ' ' << employee.getSalary() << '\n';
		}
		out.close();
		if (!out)
		{
			remove(temp.c_str());
			throw runtime_error(path + ": could not write file");
		}
	}
	if (!MoveFileExA(temp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		remove(temp.c_str());
		throw runtime_error(path + ": could not write file");
	}
}
