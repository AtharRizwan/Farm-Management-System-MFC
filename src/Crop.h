/* Header file for Crop class and its subclasses and Harvest class*/
#ifndef CROP_H
#define CROP_H

#include <string>
#include <vector>

class Harvest;

// Text used for a date that has not been set
extern const char* const kDateNotSet;

// Returns today's date as M/D/YYYY
std::string todayString();

// Crop Class
class Crop
{
	// Friend declaration
	friend class Harvest;
protected:
	std::string name;
	std::string variety;
	int fieldSize = 0;	// in acres
	double quantity = 0;	// in tonnes
	double price = 0;	// in Rs. per tonne
	std::string plantingDate = kDateNotSet;
	std::string harvestingDate = kDateNotSet;
	std::string growthStatus;

	// Read and write the fields shared by all crops, in file order
	void loadCommon(std::istream& in, const std::string& path, int& line);
	void saveCommon(std::ostream& out) const;
	// Path of the crop's data file
	std::string dataFile() const;

public:
	// Constructor
	explicit Crop(std::string name);
	virtual ~Crop() = default;

	// Growth statuses in order, from "Not Planted" to "Maturity".
	// A harvested crop has the status "Harvested" until a new season starts.
	static const std::vector<std::string>& statuses();

	// Setter functions
	void setVariety(const std::string& variety);
	void setQuantity(double quantity);
	void setPrice(double price);
	void setPlantingDate(const std::string& plantingDate);
	void setHarvestingDate(const std::string& harvestingDate);
	void setFieldSize(int fieldSize);

	// Getter Functions
	const std::string& getName() const;
	const std::string& getVariety() const;
	double getQuantity() const;
	double getPrice() const;
	const std::string& getPlantingDate() const;
	const std::string& getHarvestingDate() const;
	int getFieldSize() const;
	const std::string& getGrowthStatus() const;
	bool isMature() const;
	bool isHarvested() const;

	// Moves the crop to its next growth status.
	// Returns false if the crop has already matured.
	bool advanceStatus();

	// Tonnes that a harvest would collect: the entered quantity,
	// or the estimated yield for the whole field if none was entered
	double harvestableQuantity() const;

	// Calculates and returns revenue
	double calculateRevenue() const;

	// Calculates and returns the estimated yield in tonnes per acre.
	// The yield of each crop is calculated differently.
	virtual double calculateYield() const = 0;

	// record any pest infestations
	void recordPestInfestation(const std::string& date, const std::string& description) const;

	// record any disease outbreaks
	void recordDiseaseOutbreak(const std::string& date, const std::string& description) const;

	// Water crop from the storage and record it
	void recordCropWatering(Harvest& storage, const std::string& date, double water) const;

	// Fertilize crop from the storage and record it
	void recordFertilization(Harvest& storage, const std::string& date, double fertilizer) const;

	// Function to reset the Crop for new season
	virtual void startNewSeason();

	// Load from and save to the crop's file in data/.
	// Both throw std::runtime_error on failure.
	virtual void load() = 0;
	virtual void save() const = 0;
};


// Derived class wheat
class Wheat : public Crop
{
	int headsPerYard = 0;	// heads per square yard
	double headWeight = 0;	// in grams
public:
	Wheat();

	// setter functions
	void setHeadsPerYard(int headsPerYard);
	void setHeadWeight(double headWeight);

	// Getter functions
	int getHeadsPerYard() const;
	double getHeadWeight() const;

	double calculateYield() const override;
	void startNewSeason() override;
	void load() override;
	void save() const override;
};


// Derived class Corn
class Corn : public Crop
{
	int earsPerAcre = 0;
	int kernelsPerEar = 0;
	double kernelWeight = 0;	// in grams
	int shrinkage = 0;	// in percent
public:
	Corn();

	// Setter functions
	void setEarsPerAcre(int earsPerAcre);
	void setKernelsPerEar(int kernelsPerEar);
	void setKernelWeight(double kernelWeight);
	void setShrinkage(int shrinkage);

	// Getter functions
	int getEarsPerAcre() const;
	int getKernelsPerEar() const;
	double getKernelWeight() const;
	int getShrinkage() const;

	double calculateYield() const override;
	void startNewSeason() override;
	void load() override;
	void save() const override;
};


// Derived class Rice
class Rice : public Crop
{
	int numPaniclesPerM2 = 0;
	int grainsPerPanicle = 0;
	double grainWeight = 0;	// in grams

public:
	Rice();

	// setter functions
	void setNumPaniclesPerM2(int numPaniclesPerM2);
	void setGrainsPerPanicle(int grainsPerPanicle);
	void setGrainWeight(double grainWeight);

	// Getter functions
	int getNumPaniclesPerM2() const;
	int getGrainsPerPanicle() const;
	double getGrainWeight() const;

	double calculateYield() const override;
	void startNewSeason() override;
	void load() override;
	void save() const override;
};

class Harvest {
private:
	//friend declaration
	friend Crop;

	// data members
	double fertilizer = 0;	// in kg
	double water = 0;	// in litres
	double priceFertilizer = 0;	// in Rs per kg
	double priceWater = 0;	// in Rs per litres
	double amountRice = 0;	// in tonnes
	double amountWheat = 0;	// in tonnes
	double amountCorn = 0;	// in tonnes
	double revenue = 0;	// in Rs.

public:
	// setter functions
	void setFertilizer(double amount);
	void setWater(double amount);
	void setPriceFertilizer(double price);
	void setPriceWater(double price);

	// Getter functions
	double getFertilizer() const;
	double getWater() const;
	double getPriceFertilizer() const;
	double getPriceWater() const;
	double getAmountRice() const;
	double getAmountWheat() const;
	double getAmountCorn() const;
	double getRevenue() const;

	// Harvests the crops that have reached maturity and stores them.
	// Returns the names of the harvested crops.
	std::vector<std::string> harvestAndStore(Wheat& wheat, Corn& corn, Rice& rice);

	// Sells all stored crops at each crop's price.
	// Returns the money earned.
	double sellAndGenerateRevenue(const Wheat& wheat, const Corn& corn, const Rice& rice);

	// Load from and save to data/Harvest.txt.
	// Both throw std::runtime_error on failure.
	void load();
	void save() const;
};


#endif // !CROP_H
