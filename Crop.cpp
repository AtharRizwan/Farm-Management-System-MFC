#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Crop.h"

using namespace std;

string buffer;
fstream file;

// initialization of static members
double Crop::price = 0;
double Crop::quantity = 0;

/********************************************************************************************
 *																							*
 *						Implementation for Crop Class										*
 *																							*
 ********************************************************************************************/
// Constructor
Crop::Crop() {}

// Setter functions
void Crop::setName(string name)
{
	this->name = name;
}

void Crop::setQuantity(double quantity)
{
	if (quantity < 0) {
		throw invalid_argument("Quantity cannot be negative");
	}
	Crop::quantity = quantity;
}

void Crop::setPrice(double price)
{
	if (price < 0) {
		throw invalid_argument("Price cannot be negative");
	}
	Crop::price = price;
}

void Crop::setPlantingDate(string plantingDate)
{	
	this->plantingDate = plantingDate;
}

void Crop::setHarvestingDate(string harvestingDate)
{
	this->harvestingDate = harvestingDate;
}

void Crop::setFieldSize(int fieldSize)
{
	if (fieldSize < 0) {
		throw invalid_argument("Field size cannot be negative");
	}
	this->fieldSize = fieldSize;
}
// Getter Functions
string Crop::getName() const
{
	return name;
}

double Crop::getQuantity() const
{
	return quantity;
}

double Crop::getPrice() const
{
	return price;
}

string Crop::getPlantingDate() const
{
	return plantingDate;
}

string Crop::getHarvestingDate() const
{
	return harvestingDate;
}

int Crop::getFieldSize() const
{
	return fieldSize;
}

string Crop::getGrowthStatus() const
{
	return growthStatus;
}


// Function to advance the growth status
// The growth statuses in order are
// Germination, Seedling, Vegetative,
// Flowering, Filling, Maturity
void Crop::advanceStatus()
{
	if (growthStatus == "Not Planted") 
	{
		growthStatus = "Germination";
	}
	else if (growthStatus == "Germination") 
	{
		growthStatus = "Seedling";
	}
	else if (growthStatus == "Seedling") 
	{
		growthStatus = "Vegetative";
	}
	else if (growthStatus == "Vegetative")
	{
		growthStatus = "Flowering";
	}
	else if (growthStatus == "Flowering")
	{
		growthStatus = "Filling";
	}
	else if (growthStatus == "Filling")
	{
		growthStatus = "Maturity";
	}
}

// Function to update quantity i.e. add or remove some amount
void Crop::updateQuantity(double amount)
{
	quantity += amount;
}

// Function to update price i.e. add or remove
void Crop::updatePrice(double amount)
{
	price += amount;
}

// Calculates and returns revenue
double Crop::calculateRevenue() const
{
	return quantity * price;
}

// Get Crop Yield after harvest
double Crop::getTotalYield() const
{
	return quantity / fieldSize;
}

// Calculates and returns Yield  
double Crop::calculateYield() const
{
	return quantity / fieldSize;
}

// record any pest infestations
void Crop::recordPestInfestation(string date, string description)
{
	file.open("Pest Infestation record.txt", ios::app);
	if (!file.is_open())
	{
		throw "Unable to Open File!";
	}
	file << "Crop: " << name << endl;
	file << "Date: " << date << endl;
	file << "Description: " << description << endl << endl;
	file.close();
}

// record any disease outbreaks
void Crop::recordDiseaseOutbreak(string date, string description)
{
	file.open("Crop Disease record.txt", ios::app);
	if (!file.is_open())
	{
		throw "Unable to Open File!";
	}
	file << "Crop: " << name << endl;
	file << "Date: " << date << endl;
	file << "Description: " << description << endl << endl;
	file.close();
}


// Water crop and record it
void Crop::recordCropWatering(string date, double water)
{
	if (growthStatus == "Maturity")
	{
		throw "Crop has already matured!";
	}
	if (Harvest::water < water)
	{
		throw "Insufficient water!";
	}
	file.open("Crop Watering record.txt", ios::app);
	if (!file.is_open())
	{
		throw "Unable to Open File!";
	}
	file << "Crop: " << name << endl;
	file << "Date: " << date << endl;
	file << "Amount of water used: " << to_string(water) << endl << endl;
	file.close();
}

// Function to fertilize crop and record it
void Crop::recordFertilization(string date, double fertilizer)
{
	if (growthStatus == "Maturity")
	{
		throw "Crop has already matured!";
	}
	if (Harvest::fertilizer < fertilizer)
	{
		throw "Insufficient fertilizer!";
	}
	file.open("Crop Fertilization record.txt", ios::app);
	if (!file.is_open())
	{
		throw "Unable to Open File!";
	}
	file << "Crop: " << name << endl;
	file << "Date: " << date << endl;
	file << "Amount of fertilizer used: " << to_string(fertilizer) << endl << endl;
	file.close();
}

// Function to reset the Crop for new season
void Crop::startNewSeason()
{
	growthStatus = "Not Planted";
	fieldSize = 0;
	quantity = 0;
	plantingDate = "Not set";
	harvestingDate = "Not set";
}

/********************************************************************************************
 *																							*
 *						Implementation for Wheat Class										*
 *																							*
 ********************************************************************************************/
// Constructor
Wheat::Wheat() {
	name = "Wheat";
	// Load all data
	file.open("Wheat.txt", ios::in);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	getline(file, buffer);
	setWheatType(buffer);
	getline(file, buffer);
	setFieldSize(stoi(buffer));
	getline(file, buffer);
	setQuantity(stod(buffer));
	getline(file, buffer); 
	setPrice(stod(buffer));
	getline(file, buffer);
	growthStatus = buffer;
	getline(file, buffer);
	setPlantingDate(buffer);
	getline(file, buffer);
	setHarvestingDate(buffer);
	getline(file, buffer);
	setHeadsPerYard(stoi(buffer));
	getline(file, buffer);
	setHeadWeight(stod(buffer));
	file.close();
}
// setter functions
void Wheat::setWheatType(string wheatType)
{
	this->wheatType = wheatType;
}

void Wheat::setHeadsPerYard(int headsPerYard) {
	if (headsPerYard < 0) {
		throw invalid_argument("Heads per yard cannot be negative");
	}
	this->headsPerYard = headsPerYard;
}

void Wheat::setHeadWeight(double headWeight) {
	if (headWeight < 0.0) {
		throw invalid_argument("Head weight cannot be negative");
	}
	this->headWeight = headWeight;
}


// Getter functions
string Wheat::getWheatType() const
{
	return wheatType;
}

int Wheat::getHeadsPerYard() const
{
	return headsPerYard;
}

double Wheat::getHeadWeight() const
{
	return headWeight;
}


// Function to calculate yield in tonnes per acre
double Wheat::calculateYield() const
{
	return (headsPerYard * headWeight * 0.00002246) / 0.907;
}

// Overridden function of startNewSeason
void Wheat::startNewSeason()
{
	Crop::startNewSeason();
	headWeight = 0;
	headsPerYard = 0;
}

// Destructor for saving data
Wheat::~Wheat()
{
	// open file
	file.open("Wheat.txt", ios::out);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	// Input data
	file << wheatType << endl;
	file << fieldSize << endl;
	file << quantity << endl;
	file << price << endl;
	file << growthStatus << endl;
	file << plantingDate << endl;
	file << harvestingDate << endl;
	file << headsPerYard << endl;
	file << headWeight << endl;
	file.close();
}

/********************************************************************************************
 *																							*
 *						Implementation for Corn Class										*
 *																							*
 ********************************************************************************************/
 // Constructor
Corn::Corn()
{
	name = "Corn";
	// Load all data
	file.open("Corn.txt", ios::in);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	getline(file, buffer);
	setCornType(buffer);
	getline(file, buffer);
	setFieldSize(stoi(buffer));
	getline(file, buffer);
	setQuantity(stod(buffer));
	getline(file, buffer);
	setPrice(stod(buffer));
	getline(file, buffer);
	growthStatus = buffer;
	getline(file, buffer);
	setPlantingDate(buffer);
	getline(file, buffer);
	setHarvestingDate(buffer);
	getline(file, buffer);
	setEarsPerAcre(stoi(buffer));
	getline(file, buffer);
	setEarWeight(stod(buffer));
	getline(file, buffer);
	setKernalsPerEar(stoi(buffer));
	getline(file, buffer);
	setShrinkage(stoi(buffer));
	file.close();
}

// Setter functions
void Corn::setCornType(string cornType)
{
	this->cornType = cornType;
}

void Corn::setEarsPerAcre(int earsPerAcre) {
	if (earsPerAcre < 0) {
		throw invalid_argument("Ears per acre cannot be negative");
	}
	this->earsPerAcre = earsPerAcre;
}

void Corn::setKernalsPerEar(int kernalsPerEar) {
	if (kernalsPerEar < 0) {
		throw invalid_argument("Kernals per ear cannot be negative");
	}
	this->kernalsPerEar = kernalsPerEar;
}

void Corn::setEarWeight(double earWeight) {
	if (earWeight < 0.0) {
		throw invalid_argument("Ear weight cannot be negative");
	}
	this->earWeight = earWeight;
}

void Corn::setShrinkage(int shrinkage) {
	if (shrinkage < 0) {
		throw invalid_argument("Shrinkage cannot be negative");
	}
	this->shrinkage = shrinkage;
}

// Getter functions
string Corn::getCornType() const
{
	return cornType;
}

int Corn::getEarsPerAcre() const
{
	return earsPerAcre;
}

int Corn::getKernalsPerEar() const
{
	return kernalsPerEar;
}

double Corn::getEarWeight() const
{
	return earWeight;
}

int Corn::getShrinkage() const
{
	return shrinkage;
}

// Function to calculate yield in tonnes per acre
double Corn::calculateYield() const
{
	return (earsPerAcre * kernalsPerEar * earWeight * 0.0254) / (1000 * (1 - shrinkage * 0.01));
}

// Overridden function of startNewSeason
void Corn::startNewSeason()
{
	Crop::startNewSeason();
	earsPerAcre = 0;
	kernalsPerEar = 0;
	earWeight = 0;
	shrinkage = 0;
}

// Destructor for saving data
Corn::~Corn()
{
	// open file
	file.open("Corn.txt", ios::out);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	// Input data
	file << cornType << endl;
	file << fieldSize << endl;
	file << quantity << endl;
	file << price << endl;
	file << growthStatus << endl;
	file << plantingDate << endl;
	file << harvestingDate << endl;
	file << earsPerAcre << endl;
	file << earWeight << endl;
	file << kernalsPerEar << endl;
	file << shrinkage << endl;
	file.close();
}

/********************************************************************************************
 *																							*
 *						Implementation for Rice Class										*
 *																							*
 ********************************************************************************************/
 // Constructor
Rice::Rice()
{
	name = "Rice";
	// Load all data
	file.open("Rice.txt", ios::in);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	getline(file, buffer);
	setRiceType(buffer);
	getline(file, buffer);
	setFieldSize(stoi(buffer));
	getline(file, buffer);
	setQuantity(stod(buffer));
	getline(file, buffer);
	setPrice(stod(buffer));
	getline(file, buffer);
	growthStatus = buffer;
	getline(file, buffer);
	setPlantingDate(buffer);
	getline(file, buffer);
	setHarvestingDate(buffer);
	getline(file, buffer);
	setNumPaniclesPerM2(stoi(buffer));
	getline(file, buffer);
	setGrainsPerPanicle(stoi(buffer));
	getline(file, buffer);
	setGrainWeight(stod(buffer));
	file.close();
}

// setter functions
void Rice::setRiceType(string riceType)
{
	this->riceType = riceType;
}

void Rice::setNumPaniclesPerM2(int numPaniclesPerM2) {
	if (numPaniclesPerM2 < 0) {
		throw invalid_argument("Number of panicles per square meter cannot be negative");
	}
	this->numPaniclesPerM2 = numPaniclesPerM2;
}

void Rice::setGrainsPerPanicle(int grainsPerPanicle) {
	if (grainsPerPanicle < 0) {
		throw invalid_argument("Grains per panicle cannot be negative");
	}
	this->grainsPerPanicle = grainsPerPanicle;
}

void Rice::setGrainWeight(double grainWeight) {
	if (grainWeight < 0.0) {
		throw invalid_argument("Grain weight cannot be negative");
	}
	this->grainWeight = grainWeight;
}

// Getter functions
string Rice::getRiceType() const
{
	return riceType;
}

int Rice::getNumPaniclesPerM2() const
{
	return numPaniclesPerM2;
}

int Rice::getGrainsPerPanicle() const
{
	return grainsPerPanicle;
}

double Rice::getGrainWeight() const
{
	return grainWeight;
}

// Function to calculate yield in tonnes per acre
double Rice::calculateYield() const
{
	return (numPaniclesPerM2 * grainsPerPanicle * grainWeight) / 40.47;
}

// Overridden function of startNewSeason
void Rice::startNewSeason()
{
	Crop::startNewSeason();
	numPaniclesPerM2 = 0;
	grainsPerPanicle = 0;
	grainWeight = 0;
}

// Destructor for saving data
Rice::~Rice()
{
	// open file
	file.open("Rice.txt", ios::out);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	// Input data
	file << riceType << endl;
	file << fieldSize << endl;
	file << quantity << endl;
	file << price << endl;
	file << growthStatus << endl;
	file << plantingDate << endl;
	file << harvestingDate << endl;
	file << numPaniclesPerM2 << endl;
	file << grainsPerPanicle << endl;
	file << grainWeight << endl;
	file.close();
}

/********************************************************************************************
 *																							*
 *						Implementation for Harvest Class								    *
 *																							*
 ********************************************************************************************/

// initialization of static members
double Harvest::water = 0;
double Harvest::fertilizer = 0;


// Constructor
Harvest::Harvest()
{
	// open file
	file.open("Harvest.txt", ios::in);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	// load all data
	getline(file, buffer);
	setFertilizer(stod(buffer));
	getline(file, buffer);
	setWater(stod(buffer));
	getline(file, buffer);
	setPriceFertilizer(stod(buffer));
	getline(file, buffer);
	setPriceWater(stod(buffer));
	getline(file, buffer);
	amountRice = stod(buffer);
	getline(file, buffer);
	amountCorn = stod(buffer);
	getline(file, buffer);
	amountWheat = stod(buffer);
	getline(file, buffer);
	revenue = stod(buffer);
	file.close();
}

//setter functions

void Harvest::setFertilizer(double amount)
{
	if (amount < 0) {
		throw std::invalid_argument("Fertilizer quantity cannot be negative");
	}
	fertilizer = amount;
}

void Harvest::setWater(double amount)
{
	if (amount < 0) {
		throw std::invalid_argument("Water quantity cannot be negative");
	}
	water = amount;
}

void Harvest::setPriceFertilizer(double price)
{
	if (price < 0) {
		throw std::invalid_argument("Fertilizer price cannot be negative");
	}
	priceFertilizer = price;
}

void Harvest::setPriceWater(double price)
{
	if (price < 0) {
		throw std::invalid_argument("Water price cannot be negative");
	}
	priceWater = price;
}

void Harvest::setAmountRice(double value) {
	if (value < 0) {
		throw std::invalid_argument("Amount of rice cannot be negative.");
	}
	amountRice = value;
}

void Harvest::setAmountWheat(double value) {
	if (value < 0) {
		throw std::invalid_argument("Amount of wheat cannot be negative.");
	}
	amountWheat = value;
}

void Harvest::setAmountCorn(double value) {
	if (value < 0) {
		throw std::invalid_argument("Amount of corn cannot be negative.");
	}
	amountCorn = value;
}

void Harvest::setRevenue(double value) {
	if (value < 0) {
		throw std::invalid_argument("Revenue cannot be negative.");
	}
	revenue = value;
}


// getter functions
double Harvest::getFertilizer()
{
	return fertilizer;
}

double Harvest::getWater()
{
	return water;
}

double Harvest::getPriceFertilizer() const
{
	return priceFertilizer;
}

double Harvest::getPriceWater() const
{
	return priceWater;
}

double Harvest::getAmountRice() const
{
	return amountRice;
}

double Harvest::getAmountWheat() const
{
	return amountWheat;
}

double Harvest::getAmountCorn() const
{
	return amountCorn;
}

double Harvest::getRevenue() const
{
	return revenue;
}

// Function to update amount of water
void Harvest::updateWater(double amount)
{
	water += amount;
}

// Function to update amount of fertilizer
void Harvest::updateFertilizer(double amount)
{
	fertilizer += amount;
}

// Function to store the harvested crop
void Harvest::harvestAndStore()
{
	// Store the harvested amounts
	amountWheat += Wheat::quantity;
	amountCorn += Corn::quantity;
	amountRice += Rice::quantity;
	Rice::setQuantity(0);
	Wheat::setQuantity(0);
	Corn::setQuantity(0);
}

//Function to sell all harvested crops and return the revenue
void Harvest::SellAndGenerateRevenue()
{
	// Calculate revenue for each crop
	double cornRevenue = amountCorn * Corn::price;
	double wheatRevenue = amountWheat * Wheat::price;
	double riceRevenue = amountRice * Rice::price;
	// Sum the revenues
	revenue = cornRevenue + wheatRevenue + riceRevenue;
	// Reset the harvested amounts to zero
	amountCorn = 0;
	amountWheat = 0;
	amountRice = 0;
}

// Destructor to save data
Harvest::~Harvest()
{
	// Open file
	file.open("Harvest.txt", ios::out);
	if (!file.is_open())
	{
		throw "Unable to open file!";
	}
	// Save data
	file << fertilizer << endl;
	file << water << endl;
	file << priceFertilizer << endl;
	file << priceWater << endl;
	file << amountRice << endl;
	file << amountCorn << endl;
	file << amountWheat << endl;
	file << revenue << endl;
	file.close();
}
