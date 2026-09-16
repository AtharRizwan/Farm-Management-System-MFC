#include "pch.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "Crop.h"

using namespace std;

const char* const kDateNotSet = "Not set";

namespace
{
	const char* const kStatusNotPlanted = "Not Planted";
	const char* const kStatusMaturity = "Maturity";
	const char* const kStatusHarvested = "Harvested";

	// Throws std::runtime_error with the file name and the problem
	[[noreturn]] void fail(const string& path, const string& problem)
	{
		throw runtime_error(path + ": " + problem);
	}

	string trim(const string& s)
	{
		const char* ws = " \t\r\n";
		size_t first = s.find_first_not_of(ws);
		if (first == string::npos)
		{
			return "";
		}
		return s.substr(first, s.find_last_not_of(ws) - first + 1);
	}

	string readLine(istream& in, const string& path, int& line)
	{
		string buffer;
		++line;
		if (!getline(in, buffer))
		{
			fail(path, "missing value on line " + to_string(line));
		}
		return trim(buffer);
	}

	double readDouble(istream& in, const string& path, int& line)
	{
		string text = readLine(in, path, line);
		try
		{
			size_t used = 0;
			double value = stod(text, &used);
			if (used == text.size() && isfinite(value))
			{
				return value;
			}
		}
		catch (const logic_error&)
		{
		}
		fail(path, "line " + to_string(line) + " is not a number: \"" + text + "\"");
	}

	int readInt(istream& in, const string& path, int& line)
	{
		string text = readLine(in, path, line);
		try
		{
			size_t used = 0;
			int value = stoi(text, &used);
			if (used == text.size())
			{
				return value;
			}
		}
		catch (const logic_error&)
		{
		}
		fail(path, "line " + to_string(line) + " is not a whole number: \"" + text + "\"");
	}

	// Calls a setter and reports a rejected value with its line number
	template <typename Setter>
	void applyValue(const string& path, int line, Setter setter)
	{
		try
		{
			setter();
		}
		catch (const invalid_argument& e)
		{
			fail(path, "line " + to_string(line) + ": " + e.what());
		}
	}

	ifstream openForReading(const string& path)
	{
		ifstream in(path);
		if (!in)
		{
			fail(path, "cannot open file for reading");
		}
		return in;
	}

	// Data files are written to a temporary file first, which then replaces
	// the real one, so a failed save never leaves a half-written file.
	string tempPath(const string& path)
	{
		return path + ".tmp";
	}

	ofstream openForWriting(const string& path)
	{
		ofstream out(tempPath(path), ios::trunc);
		if (!out)
		{
			fail(path, "cannot open file for writing");
		}
		out.precision(15);
		return out;
	}

	void finishWriting(ofstream& out, const string& path)
	{
		out.close();
		if (!out || !MoveFileExA(tempPath(path).c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
		{
			remove(tempPath(path).c_str());
			fail(path, "could not write file");
		}
	}

	void appendRecord(const string& path, const string& crop, const string& date, const string& detail)
	{
		ofstream out(path, ios::app);
		if (!out)
		{
			fail(path, "cannot open file for writing");
		}
		out << "Crop: " << crop << '\n';
		out << "Date: " << date << '\n';
		out << detail << "\n\n";
		out.close();
		if (!out)
		{
			fail(path, "could not write file");
		}
	}

	// Dates are stored as M/D/YYYY, or "Not set"
	string normalizeDate(const string& date)
	{
		string text = trim(date);
		string lower = text;
		transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return static_cast<char>(tolower(c)); });
		if (text.empty() || lower == "not set")
		{
			return kDateNotSet;
		}
		return text;
	}

	void requireNonNegative(double value, const char* what)
	{
		if (value < 0 || !isfinite(value))
		{
			throw invalid_argument(string(what) + " cannot be negative");
		}
	}

	string formatAmount(double value)
	{
		ostringstream out;
		out << value;
		return out.str();
	}
}

string todayString()
{
	time_t now = time(nullptr);
	tm local{};
	localtime_s(&local, &now);
	return to_string(local.tm_mon + 1) + "/" + to_string(local.tm_mday) + "/" + to_string(local.tm_year + 1900);
}

/********************************************************************************************
 *																							*
 *						Implementation for Crop Class										*
 *																							*
 ********************************************************************************************/
Crop::Crop(string name)
	: name(move(name)), growthStatus(kStatusNotPlanted)
{
}

const vector<string>& Crop::statuses()
{
	static const vector<string> list = {
		kStatusNotPlanted, "Germination", "Seedling", "Vegetative",
		"Flowering", "Filling", kStatusMaturity, kStatusHarvested
	};
	return list;
}

// Setter functions
void Crop::setVariety(const string& variety)
{
	this->variety = variety;
}

void Crop::setQuantity(double quantity)
{
	requireNonNegative(quantity, "Quantity");
	this->quantity = quantity;
}

void Crop::setPrice(double price)
{
	requireNonNegative(price, "Price");
	this->price = price;
}

void Crop::setPlantingDate(const string& plantingDate)
{
	this->plantingDate = normalizeDate(plantingDate);
}

void Crop::setHarvestingDate(const string& harvestingDate)
{
	this->harvestingDate = normalizeDate(harvestingDate);
}

void Crop::setFieldSize(int fieldSize)
{
	if (fieldSize < 0) {
		throw invalid_argument("Field size cannot be negative");
	}
	this->fieldSize = fieldSize;
}

// Getter Functions
const string& Crop::getName() const
{
	return name;
}

const string& Crop::getVariety() const
{
	return variety;
}

double Crop::getQuantity() const
{
	return quantity;
}

double Crop::getPrice() const
{
	return price;
}

const string& Crop::getPlantingDate() const
{
	return plantingDate;
}

const string& Crop::getHarvestingDate() const
{
	return harvestingDate;
}

int Crop::getFieldSize() const
{
	return fieldSize;
}

const string& Crop::getGrowthStatus() const
{
	return growthStatus;
}

bool Crop::isMature() const
{
	return growthStatus == kStatusMaturity;
}

bool Crop::isHarvested() const
{
	return growthStatus == kStatusHarvested;
}

bool Crop::advanceStatus()
{
	const vector<string>& list = statuses();
	auto current = find(list.begin(), list.end(), growthStatus);
	auto maturity = find(list.begin(), list.end(), kStatusMaturity);
	if (current == list.end() || current >= maturity)
	{
		return false;
	}
	growthStatus = *(current + 1);
	return true;
}

double Crop::harvestableQuantity() const
{
	if (quantity > 0)
	{
		return quantity;
	}
	return calculateYield() * fieldSize;
}

// Calculates and returns revenue
double Crop::calculateRevenue() const
{
	return quantity * price;
}

// record any pest infestations
void Crop::recordPestInfestation(const string& date, const string& description) const
{
	if (trim(description).empty())
	{
		throw invalid_argument("Please describe the pest infestation");
	}
	appendRecord("data/Pest Infestation record.txt", name, date, "Description: " + description);
}

// record any disease outbreaks
void Crop::recordDiseaseOutbreak(const string& date, const string& description) const
{
	if (trim(description).empty())
	{
		throw invalid_argument("Please describe the disease outbreak");
	}
	appendRecord("data/Crop Disease record.txt", name, date, "Description: " + description);
}

// Water crop from the storage and record it
void Crop::recordCropWatering(Harvest& storage, const string& date, double water) const
{
	if (isMature() || isHarvested())
	{
		throw invalid_argument(name + " has already matured");
	}
	if (!(water > 0))
	{
		throw invalid_argument("Amount of water must be more than zero");
	}
	if (storage.water < water)
	{
		throw invalid_argument("Not enough water in storage (" + formatAmount(storage.water) + " litres left)");
	}
	appendRecord("data/Crop Watering record.txt", name, date, "Amount of water used: " + formatAmount(water) + " litres");
	storage.water -= water;
}

// Fertilize crop from the storage and record it
void Crop::recordFertilization(Harvest& storage, const string& date, double fertilizer) const
{
	if (isMature() || isHarvested())
	{
		throw invalid_argument(name + " has already matured");
	}
	if (!(fertilizer > 0))
	{
		throw invalid_argument("Amount of fertilizer must be more than zero");
	}
	if (storage.fertilizer < fertilizer)
	{
		throw invalid_argument("Not enough fertilizer in storage (" + formatAmount(storage.fertilizer) + " kg left)");
	}
	appendRecord("data/Crop Fertilization record.txt", name, date, "Amount of fertilizer used: " + formatAmount(fertilizer) + " kg");
	storage.fertilizer -= fertilizer;
}

// Function to reset the Crop for new season
void Crop::startNewSeason()
{
	growthStatus = kStatusNotPlanted;
	fieldSize = 0;
	quantity = 0;
	plantingDate = kDateNotSet;
	harvestingDate = kDateNotSet;
}

string Crop::dataFile() const
{
	return "data/" + name + ".txt";
}

// File order: variety, field size, quantity, price, status, planting date, harvesting date
void Crop::loadCommon(istream& in, const string& path, int& line)
{
	variety = readLine(in, path, line);
	int size = readInt(in, path, line);
	applyValue(path, line, [&] { setFieldSize(size); });
	double amount = readDouble(in, path, line);
	applyValue(path, line, [&] { setQuantity(amount); });
	double cost = readDouble(in, path, line);
	applyValue(path, line, [&] { setPrice(cost); });
	string status = readLine(in, path, line);
	const vector<string>& list = statuses();
	if (find(list.begin(), list.end(), status) == list.end())
	{
		fail(path, "line " + to_string(line) + " is not a growth status: \"" + status + "\"");
	}
	growthStatus = status;
	setPlantingDate(readLine(in, path, line));
	setHarvestingDate(readLine(in, path, line));
}

void Crop::saveCommon(ostream& out) const
{
	out << variety << '\n';
	out << fieldSize << '\n';
	out << quantity << '\n';
	out << price << '\n';
	out << growthStatus << '\n';
	out << plantingDate << '\n';
	out << harvestingDate << '\n';
}

/********************************************************************************************
 *																							*
 *						Implementation for Wheat Class										*
 *																							*
 ********************************************************************************************/
Wheat::Wheat() : Crop("Wheat") {}

void Wheat::setHeadsPerYard(int headsPerYard) {
	if (headsPerYard < 0) {
		throw invalid_argument("Heads per yard cannot be negative");
	}
	this->headsPerYard = headsPerYard;
}

void Wheat::setHeadWeight(double headWeight) {
	requireNonNegative(headWeight, "Head weight");
	this->headWeight = headWeight;
}

int Wheat::getHeadsPerYard() const
{
	return headsPerYard;
}

double Wheat::getHeadWeight() const
{
	return headWeight;
}

// Yield in tonnes per acre: grams per square yard, times 4840 square yards per acre
double Wheat::calculateYield() const
{
	return headsPerYard * headWeight * 4840.0 / 1e6;
}

void Wheat::startNewSeason()
{
	Crop::startNewSeason();
	headWeight = 0;
	headsPerYard = 0;
}

void Wheat::load()
{
	const string path = dataFile();
	ifstream in = openForReading(path);
	int line = 0;
	loadCommon(in, path, line);
	int heads = readInt(in, path, line);
	applyValue(path, line, [&] { setHeadsPerYard(heads); });
	double weight = readDouble(in, path, line);
	applyValue(path, line, [&] { setHeadWeight(weight); });
}

void Wheat::save() const
{
	const string path = dataFile();
	ofstream out = openForWriting(path);
	saveCommon(out);
	out << headsPerYard << '\n';
	out << headWeight << '\n';
	finishWriting(out, path);
}

/********************************************************************************************
 *																							*
 *						Implementation for Corn Class										*
 *																							*
 ********************************************************************************************/
Corn::Corn() : Crop("Corn") {}

void Corn::setEarsPerAcre(int earsPerAcre) {
	if (earsPerAcre < 0) {
		throw invalid_argument("Ears per acre cannot be negative");
	}
	this->earsPerAcre = earsPerAcre;
}

void Corn::setKernelsPerEar(int kernelsPerEar) {
	if (kernelsPerEar < 0) {
		throw invalid_argument("Kernels per ear cannot be negative");
	}
	this->kernelsPerEar = kernelsPerEar;
}

void Corn::setKernelWeight(double kernelWeight) {
	requireNonNegative(kernelWeight, "Kernel weight");
	this->kernelWeight = kernelWeight;
}

void Corn::setShrinkage(int shrinkage) {
	if (shrinkage < 0 || shrinkage > 99) {
		throw invalid_argument("Shrinkage must be between 0 and 99 percent");
	}
	this->shrinkage = shrinkage;
}

int Corn::getEarsPerAcre() const
{
	return earsPerAcre;
}

int Corn::getKernelsPerEar() const
{
	return kernelsPerEar;
}

double Corn::getKernelWeight() const
{
	return kernelWeight;
}

int Corn::getShrinkage() const
{
	return shrinkage;
}

// Yield in tonnes per acre, less the weight lost to shrinkage
double Corn::calculateYield() const
{
	return static_cast<double>(earsPerAcre) * kernelsPerEar * kernelWeight / 1e6 * (1 - shrinkage / 100.0);
}

void Corn::startNewSeason()
{
	Crop::startNewSeason();
	earsPerAcre = 0;
	kernelsPerEar = 0;
	kernelWeight = 0;
	shrinkage = 0;
}

// File order after the common fields: ears per acre, kernel weight, kernels per ear, shrinkage
void Corn::load()
{
	const string path = dataFile();
	ifstream in = openForReading(path);
	int line = 0;
	loadCommon(in, path, line);
	int ears = readInt(in, path, line);
	applyValue(path, line, [&] { setEarsPerAcre(ears); });
	double weight = readDouble(in, path, line);
	applyValue(path, line, [&] { setKernelWeight(weight); });
	int kernels = readInt(in, path, line);
	applyValue(path, line, [&] { setKernelsPerEar(kernels); });
	int shrink = readInt(in, path, line);
	applyValue(path, line, [&] { setShrinkage(shrink); });
}

void Corn::save() const
{
	const string path = dataFile();
	ofstream out = openForWriting(path);
	saveCommon(out);
	out << earsPerAcre << '\n';
	out << kernelWeight << '\n';
	out << kernelsPerEar << '\n';
	out << shrinkage << '\n';
	finishWriting(out, path);
}

/********************************************************************************************
 *																							*
 *						Implementation for Rice Class										*
 *																							*
 ********************************************************************************************/
Rice::Rice() : Crop("Rice") {}

void Rice::setNumPaniclesPerM2(int numPaniclesPerM2) {
	if (numPaniclesPerM2 < 0) {
		throw invalid_argument("Panicles per square metre cannot be negative");
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
	requireNonNegative(grainWeight, "Grain weight");
	this->grainWeight = grainWeight;
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

// Yield in tonnes per acre: grams per square metre, times 4046.86 square metres per acre
double Rice::calculateYield() const
{
	return static_cast<double>(numPaniclesPerM2) * grainsPerPanicle * grainWeight * 4046.86 / 1e6;
}

void Rice::startNewSeason()
{
	Crop::startNewSeason();
	numPaniclesPerM2 = 0;
	grainsPerPanicle = 0;
	grainWeight = 0;
}

void Rice::load()
{
	const string path = dataFile();
	ifstream in = openForReading(path);
	int line = 0;
	loadCommon(in, path, line);
	int panicles = readInt(in, path, line);
	applyValue(path, line, [&] { setNumPaniclesPerM2(panicles); });
	int grains = readInt(in, path, line);
	applyValue(path, line, [&] { setGrainsPerPanicle(grains); });
	double weight = readDouble(in, path, line);
	applyValue(path, line, [&] { setGrainWeight(weight); });
}

void Rice::save() const
{
	const string path = dataFile();
	ofstream out = openForWriting(path);
	saveCommon(out);
	out << numPaniclesPerM2 << '\n';
	out << grainsPerPanicle << '\n';
	out << grainWeight << '\n';
	finishWriting(out, path);
}

/********************************************************************************************
 *																							*
 *						Implementation for Harvest Class								    *
 *																							*
 ********************************************************************************************/
void Harvest::setFertilizer(double amount)
{
	requireNonNegative(amount, "Fertilizer quantity");
	fertilizer = amount;
}

void Harvest::setWater(double amount)
{
	requireNonNegative(amount, "Water quantity");
	water = amount;
}

void Harvest::setPriceFertilizer(double price)
{
	requireNonNegative(price, "Fertilizer price");
	priceFertilizer = price;
}

void Harvest::setPriceWater(double price)
{
	requireNonNegative(price, "Water price");
	priceWater = price;
}

double Harvest::getFertilizer() const
{
	return fertilizer;
}

double Harvest::getWater() const
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

vector<string> Harvest::harvestAndStore(Wheat& wheat, Corn& corn, Rice& rice)
{
	vector<string> harvested;
	auto collect = [&](Crop& crop, double& store) {
		if (!crop.isMature())
		{
			return;
		}
		store += crop.harvestableQuantity();
		crop.quantity = 0;
		crop.growthStatus = kStatusHarvested;
		if (crop.harvestingDate == kDateNotSet)
		{
			crop.harvestingDate = todayString();
		}
		harvested.push_back(crop.name);
	};
	collect(wheat, amountWheat);
	collect(corn, amountCorn);
	collect(rice, amountRice);
	return harvested;
}

double Harvest::sellAndGenerateRevenue(const Wheat& wheat, const Corn& corn, const Rice& rice)
{
	double earned = amountWheat * wheat.price + amountCorn * corn.price + amountRice * rice.price;
	revenue += earned;
	amountCorn = 0;
	amountWheat = 0;
	amountRice = 0;
	return earned;
}

// File order: fertilizer, water, fertilizer price, water price, rice, corn, wheat, revenue
void Harvest::load()
{
	const string path = "data/Harvest.txt";
	ifstream in = openForReading(path);
	int line = 0;
	double values[8];
	for (double& value : values)
	{
		value = readDouble(in, path, line);
		if (value < 0)
		{
			fail(path, "line " + to_string(line) + " cannot be negative");
		}
	}
	fertilizer = values[0];
	water = values[1];
	priceFertilizer = values[2];
	priceWater = values[3];
	amountRice = values[4];
	amountCorn = values[5];
	amountWheat = values[6];
	revenue = values[7];
}

void Harvest::save() const
{
	const string path = "data/Harvest.txt";
	ofstream out = openForWriting(path);
	out << fertilizer << '\n';
	out << water << '\n';
	out << priceFertilizer << '\n';
	out << priceWater << '\n';
	out << amountRice << '\n';
	out << amountCorn << '\n';
	out << amountWheat << '\n';
	out << revenue << '\n';
	finishWriting(out, path);
}
