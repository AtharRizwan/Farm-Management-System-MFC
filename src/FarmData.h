#pragma once
#include <string>
#include <vector>
#include "Crop.h"
#include "employee.h"

// All of the farm's data, loaded from and saved to the files in data/.
// Load and save functions throw std::runtime_error on failure.
struct FarmData
{
	Wheat wheat;
	Corn corn;
	Rice rice;
	Harvest harvest;
	std::vector<Employee> employees;

	void loadAll();
	void saveCrops() const;
	void saveEmployees() const;
};

// The farm data owned by the application
FarmData& GetFarm();
