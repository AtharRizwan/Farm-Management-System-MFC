/* Header file for Crop class and its subclasses and Harvest class*/
using namespace std;

#ifndef CROP_H
#define CROP_H

// Crop Class
class Crop
{
	// Friend declaration
	friend class Harvest;
	// Private data members
protected:
	string name;
	int fieldSize;	// in acres
	static double quantity;	// in tonnes
	static double price;	// in Rs. per tonne
	string plantingDate;
	string harvestingDate;
	string growthStatus;
	// Public member functions
public:
	// Constructor
	Crop();

	// Setter functions
	void setName(string name);
	static void setQuantity(double quantity);
	static void setPrice(double price);
	void setPlantingDate(string plantingDate);
	void setHarvestingDate(string harvestingDate);
	void setFieldSize(int fieldSize);

	// Getter Functions
	string getName() const; 
	double getQuantity() const;
	double getPrice() const;
	string getPlantingDate() const;
	string getHarvestingDate() const;
	int getFieldSize() const;
	string getGrowthStatus() const;


	// Function to advance the growth status
	// The growth statuses in order are
	// Germination, Seedling, Vegetative,
	// Flowering, Filling, Maturity
	void advanceStatus();

	// Function to update quantity i.e. add or remove
	void updateQuantity(double amount);

	// Function to update price i.e. add or remove
	void updatePrice(double amount);

	// Calculates and returns revenue
	double calculateRevenue() const;

	// Get Crop Yield after harvest
	double getTotalYield() const;

	// Calculates and returns Yield in tonnes per acre
	// The yeild of each crop is calculated differently
	virtual double calculateYield() const = 0;

	// record any pest infestations
	void recordPestInfestation(string date, string description);

	// record any disease outbreaks
	void recordDiseaseOutbreak(string date, string description);

	// Water crop and record it
	void recordCropWatering(string date, double water);

	// Function to fertilize crop and record it
	void recordFertilization(string date, double fertilizer);

	// Function to reset the Crop for new season
	void virtual startNewSeason();
};


// Derived class wheat
class Wheat : public Crop 
{
	string wheatType;
	int headsPerYard;
	double headWeight;	// in grams
public:
	// Constructor
	Wheat();

	// setter functions
	void setWheatType(string wheatType);
	void setHeadsPerYard(int headsPerYard);
	void setHeadWeight(double headWeight);

	// Getter functions
	string getWheatType() const;
	int getHeadsPerYard() const;
	double getHeadWeight() const;

	// function to calculate yield in tonnes per acres
	double calculateYield() const override;

	// Overridden function of startNewSeason
	void startNewSeason() override;

	// Destructor for saving data
	~Wheat();
};


// Derived class Corn
class Corn : public Crop
{	
	string cornType;
	int earsPerAcre;
	int kernalsPerEar;
	double earWeight;	//in grams
	int shrinkage;
public:
	// Constructor
	Corn();

	// Setter functions
	void setCornType(string cornType);
	void setEarsPerAcre(int earsPerAcre);
	void setKernalsPerEar(int kernalsPerEar);
	void setEarWeight(double earWeight);
	void setShrinkage(int shrikage);

	// Getter functions
	string getCornType() const;
	int getEarsPerAcre() const;
	int getKernalsPerEar() const;
	double getEarWeight() const;
	int getShrinkage() const;

	// Function to calculate yield
	double calculateYield() const override;

	// Overridden function of startNewSeason
	void startNewSeason() override;

	// Destructor for saving data
	~Corn();
};


// Derived class Rice
class Rice : public Crop
{
	string riceType;
	int numPaniclesPerM2;
	int grainsPerPanicle;
	double grainWeight;	//in grams

public:
	// Constructor
	Rice();

	// setter functions
	void setRiceType(string riceType);
	void setNumPaniclesPerM2(int numPaniclesPerM2);
	void setGrainsPerPanicle(int grainsPerPanicle);
	void setGrainWeight(double grainWeight);

	// Getter functions
	string setRiceType() const;
	int setNumPaniclesPerM2() const;
	int setGrainsPerPanicle() const;
	double setGrainWeight() const;

	// Function to calculate yield in tonnes per acre
	double calculateYield() const override;

	// Overridden function of startNewSeason
	void startNewSeason() override;

	// Destructor for saving data
	~Rice();
};

class Harvest {
private:
	//friend declaration
	friend Crop;

	// data members
	static double fertilizer;	// in kg
	static double water;	// in litres
	double priceFertilizer;	// in Rs per kg
	double priceWater;	// in Rs per litres
	double amountRice;	// in tonnes
	double amountWheat;	// in tonnes
	double amountCorn;	// in tonnes
	double revenue;	// in Rs.

public:
	// Constructor to load data
	Harvest();

	// setter functions
	static void setFertilizer(double amount);
	static void setWater(double amount);
	void setPriceFertilizer(double price);
	void setPriceWater(double price);

	static double getFertilizer();
	static double getWater();
	double getPriceFertilizer() const;
	double getPriceWater() const;
	double getAmountRice() const;
	double getAmountWheat() const;
	double getAmountCorn() const;
	double getRevenue() const;

	// Function to update amount of water
	void updateWater(double amount);

	// Function to update amount of fertilizer
	void updateFertilizer(double amount);

	// Function to store the harvested crop
	void harvestAndStore();

	//Function to sell all harvested crops and return the revenue
	void SellAndGenerateRevenue();

	// Desrtructor to save data
	~Harvest();
};


#endif // !CROP_H
