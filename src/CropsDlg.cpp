// CropsDlg.cpp : implementation file
//

#include "pch.h"
#include "Farm Management System MFC.h"
#include "CropsDlg.h"
#include "FieldLogDlg.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	const CropsDlg::CropIds kCropIds[3] = {
		{ IDC_WHEAT_VARIETY, IDC_WHEAT_FIELD_SIZE, IDC_WHEAT_QUANTITY, IDC_WHEAT_PRICE,
		  IDC_WHEAT_STATUS, IDC_WHEAT_PLANTED, IDC_WHEAT_HARVESTED, IDC_WHEAT_YIELD },
		{ IDC_CORN_VARIETY, IDC_CORN_FIELD_SIZE, IDC_CORN_QUANTITY, IDC_CORN_PRICE,
		  IDC_CORN_STATUS, IDC_CORN_PLANTED, IDC_CORN_HARVESTED, IDC_CORN_YIELD },
		{ IDC_RICE_VARIETY, IDC_RICE_FIELD_SIZE, IDC_RICE_QUANTITY, IDC_RICE_PRICE,
		  IDC_RICE_STATUS, IDC_RICE_PLANTED, IDC_RICE_HARVESTED, IDC_RICE_YIELD },
	};

	const LPCTSTR kHint = _T("Press Save to keep your changes. Action buttons also save.");

	// Returns true if date a is earlier than date b
	bool IsBefore(const SYSTEMTIME& a, const SYSTEMTIME& b)
	{
		FILETIME fa;
		FILETIME fb;
		return SystemTimeToFileTime(&a, &fa) && SystemTimeToFileTime(&b, &fb) && CompareFileTime(&fa, &fb) < 0;
	}

	CString JoinNames(const std::vector<std::string>& names)
	{
		CString result;
		for (size_t i = 0; i < names.size(); ++i)
		{
			if (i > 0)
			{
				result += (i + 1 == names.size()) ? _T(" and ") : _T(", ");
			}
			result += ToCString(names[i]);
		}
		return result;
	}
}

// CropsDlg dialog

IMPLEMENT_DYNAMIC(CropsDlg, ThemedDialog)

CropsDlg::CropsDlg(CWnd* pParent /*=nullptr*/)
	: ThemedDialog(IDD_CROPS, pParent)
{
}

void CropsDlg::DoDataExchange(CDataExchange* pDX)
{
	ThemedDialog::DoDataExchange(pDX);
	for (int i = 0; i < 3; ++i)
	{
		DDX_Control(pDX, kCropIds[i].planted, m_planted[i]);
		DDX_Control(pDX, kCropIds[i].harvested, m_harvested[i]);
	}
}


BEGIN_MESSAGE_MAP(CropsDlg, ThemedDialog)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_WHEAT_VARIETY, IDC_WATER_PRICE, &CropsDlg::OnFieldChanged)
	ON_NOTIFY_RANGE(DTN_DATETIMECHANGE, IDC_WHEAT_VARIETY, IDC_RICE_GRAIN_WEIGHT, &CropsDlg::OnDateChanged)
	ON_BN_CLICKED(IDC_WHEAT_ADVANCE, &CropsDlg::OnAdvanceWheat)
	ON_BN_CLICKED(IDC_CORN_ADVANCE, &CropsDlg::OnAdvanceCorn)
	ON_BN_CLICKED(IDC_RICE_ADVANCE, &CropsDlg::OnAdvanceRice)
	ON_BN_CLICKED(IDC_WHEAT_FIELD_LOG, &CropsDlg::OnFieldLogWheat)
	ON_BN_CLICKED(IDC_CORN_FIELD_LOG, &CropsDlg::OnFieldLogCorn)
	ON_BN_CLICKED(IDC_RICE_FIELD_LOG, &CropsDlg::OnFieldLogRice)
	ON_BN_CLICKED(IDC_HARVEST, &CropsDlg::OnHarvest)
	ON_BN_CLICKED(IDC_SELL, &CropsDlg::OnSell)
	ON_BN_CLICKED(IDC_NEW_SEASON, &CropsDlg::OnNewSeason)
END_MESSAGE_MAP()


Crop& CropsDlg::CropAt(FarmData& farm, int index)
{
	switch (index)
	{
	case 0:
		return farm.wheat;
	case 1:
		return farm.corn;
	default:
		return farm.rice;
	}
}

BOOL CropsDlg::OnInitDialog()
{
	ThemedDialog::OnInitDialog();

	for (int i = 0; i < 3; ++i)
	{
		SendDlgItemMessage(kCropIds[i].variety, EM_SETLIMITTEXT, 40);
		m_planted[i].SetFormat(_T("yyyy-MM-dd"));
		m_harvested[i].SetFormat(_T("yyyy-MM-dd"));
	}
	SetDlgItemText(IDC_SUBTITLE, kHint);
	LoadControls();
	return TRUE;
}

// CropsDlg message handlers

void CropsDlg::LoadControls()
{
	m_loading = true;
	FarmData& farm = GetFarm();
	for (int i = 0; i < 3; ++i)
	{
		const Crop& crop = CropAt(farm, i);
		const CropIds& ids = kCropIds[i];
		SetDlgItemText(ids.variety, ToCString(crop.getVariety()));
		SetDlgItemInt(ids.fieldSize, crop.getFieldSize(), FALSE);
		SetDlgItemText(ids.quantity, FormatNumber(crop.getQuantity()));
		SetDlgItemText(ids.price, FormatNumber(crop.getPrice(), 2));
		SetDlgItemText(ids.status, ToCString(crop.getGrowthStatus()));
		ShowDate(m_planted[i], crop.getPlantingDate());
		ShowDate(m_harvested[i], crop.getHarvestingDate());
	}

	SetDlgItemInt(IDC_WHEAT_HEADS, farm.wheat.getHeadsPerYard(), FALSE);
	SetDlgItemText(IDC_WHEAT_HEAD_WEIGHT, FormatNumber(farm.wheat.getHeadWeight()));
	SetDlgItemInt(IDC_CORN_EARS, farm.corn.getEarsPerAcre(), FALSE);
	SetDlgItemInt(IDC_CORN_KERNELS, farm.corn.getKernelsPerEar(), FALSE);
	SetDlgItemText(IDC_CORN_KERNEL_WEIGHT, FormatNumber(farm.corn.getKernelWeight()));
	SetDlgItemInt(IDC_CORN_SHRINKAGE, farm.corn.getShrinkage(), FALSE);
	SetDlgItemInt(IDC_RICE_PANICLES, farm.rice.getNumPaniclesPerM2(), FALSE);
	SetDlgItemInt(IDC_RICE_GRAINS, farm.rice.getGrainsPerPanicle(), FALSE);
	SetDlgItemText(IDC_RICE_GRAIN_WEIGHT, FormatNumber(farm.rice.getGrainWeight()));

	const Harvest& harvest = farm.harvest;
	SetDlgItemText(IDC_FERTILIZER, FormatNumber(harvest.getFertilizer()));
	SetDlgItemText(IDC_WATER, FormatNumber(harvest.getWater()));
	SetDlgItemText(IDC_FERTILIZER_PRICE, FormatNumber(harvest.getPriceFertilizer(), 2));
	SetDlgItemText(IDC_WATER_PRICE, FormatNumber(harvest.getPriceWater(), 2));
	SetDlgItemText(IDC_STORED_WHEAT, FormatNumber(harvest.getAmountWheat()));
	SetDlgItemText(IDC_STORED_CORN, FormatNumber(harvest.getAmountCorn()));
	SetDlgItemText(IDC_STORED_RICE, FormatNumber(harvest.getAmountRice()));
	SetDlgItemText(IDC_REVENUE, FormatMoney(harvest.getRevenue()));

	UpdateYieldPreview();
	m_loading = false;
	m_dirty = false;
}

// Shows the estimated yield for the values currently typed in
void CropsDlg::UpdateYieldPreview()
{
	FarmData preview = GetFarm();
	const bool ok[3] = { ReadWheat(preview), ReadCorn(preview), ReadRice(preview) };
	m_error.Empty();
	m_errorControl = 0;

	bool wasLoading = m_loading;
	m_loading = true;
	for (int i = 0; i < 3; ++i)
	{
		CString text = _T("-");
		if (ok[i])
		{
			text = FormatNumber(CropAt(preview, i).calculateYield(), 2);
		}
		SetDlgItemText(kCropIds[i].yield, text);

		const Crop& crop = CropAt(GetFarm(), i);
		GetDlgItem(i == 0 ? IDC_WHEAT_ADVANCE : i == 1 ? IDC_CORN_ADVANCE : IDC_RICE_ADVANCE)
			->EnableWindow(!crop.isMature() && !crop.isHarvested());
	}
	m_loading = wasLoading;
}

void CropsDlg::OnFieldChanged(UINT id)
{
	if (m_loading)
	{
		return;
	}
	for (const CropIds& ids : kCropIds)
	{
		if (id == ids.status || id == ids.yield)
		{
			return;
		}
	}
	m_dirty = true;
	SetDlgItemText(IDC_SUBTITLE, kHint);
	UpdateYieldPreview();
}

void CropsDlg::OnDateChanged(UINT /*id*/, NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	if (!m_loading)
	{
		m_dirty = true;
		SetDlgItemText(IDC_SUBTITLE, kHint);
	}
	*pResult = 0;
}

bool CropsDlg::Fail(UINT id, const CString& message)
{
	m_errorControl = id;
	m_error = message;
	return false;
}

// The hint and the error message share the space above the Save button
void CropsDlg::ShowStatus(const CString& error)
{
	ShowError(error);
	GetDlgItem(IDC_SUBTITLE)->ShowWindow(error.IsEmpty() ? SW_SHOW : SW_HIDE);
}

void CropsDlg::ShowFormError()
{
	ShowStatus(m_error);
	if (CWnd* control = GetDlgItem(m_errorControl))
	{
		GotoDlgCtrl(control);
	}
}

bool CropsDlg::ReadNumber(UINT id, const CString& label, double& value)
{
	CString error;
	return ReadDouble(*this, id, label, value, error) || Fail(id, error);
}

bool CropsDlg::ReadWhole(UINT id, const CString& label, int& value)
{
	CString error;
	return ReadInt(*this, id, label, value, error) || Fail(id, error);
}

bool CropsDlg::Apply(UINT id, const CString& label, const std::function<void()>& setter)
{
	try
	{
		setter();
		return true;
	}
	catch (const std::invalid_argument& e)
	{
		return Fail(id, label + _T(": ") + ToCString(e.what()) + _T("."));
	}
}

bool CropsDlg::ReadCommon(Crop& crop, int index)
{
	const CropIds& ids = kCropIds[index];
	const CString name = ToCString(crop.getName());

	CString variety;
	GetDlgItemText(ids.variety, variety);
	variety.Trim();
	if (variety.IsEmpty())
	{
		return Fail(ids.variety, name + _T(" variety is required."));
	}

	int fieldSize = 0;
	double quantity = 0;
	double price = 0;
	if (!ReadWhole(ids.fieldSize, name + _T(" field size"), fieldSize)
		|| !ReadNumber(ids.quantity, name + _T(" quantity"), quantity)
		|| !ReadNumber(ids.price, name + _T(" price"), price))
	{
		return false;
	}

	std::string planted = ReadDate(m_planted[index]);
	std::string harvested = ReadDate(m_harvested[index]);
	SYSTEMTIME plantedDate{};
	SYSTEMTIME harvestedDate{};
	if (ParseDate(planted, plantedDate) && ParseDate(harvested, harvestedDate) && IsBefore(harvestedDate, plantedDate))
	{
		return Fail(ids.harvested, name + _T(" cannot be harvested before it was planted."));
	}

	crop.setVariety(ToUtf8(variety));
	crop.setFieldSize(fieldSize);
	crop.setQuantity(quantity);
	crop.setPrice(price);
	crop.setPlantingDate(planted);
	crop.setHarvestingDate(harvested);
	return true;
}

bool CropsDlg::ReadWheat(FarmData& farm)
{
	int heads = 0;
	double weight = 0;
	return ReadWhole(IDC_WHEAT_HEADS, _T("Wheat heads per square yard"), heads)
		&& ReadNumber(IDC_WHEAT_HEAD_WEIGHT, _T("Wheat head weight"), weight)
		&& Apply(IDC_WHEAT_HEADS, _T("Wheat"), [&] {
			farm.wheat.setHeadsPerYard(heads);
			farm.wheat.setHeadWeight(weight);
		});
}

bool CropsDlg::ReadCorn(FarmData& farm)
{
	int ears = 0;
	int kernels = 0;
	double weight = 0;
	int shrinkage = 0;
	return ReadWhole(IDC_CORN_EARS, _T("Corn ears per acre"), ears)
		&& ReadWhole(IDC_CORN_KERNELS, _T("Corn kernels per ear"), kernels)
		&& ReadNumber(IDC_CORN_KERNEL_WEIGHT, _T("Corn kernel weight"), weight)
		&& ReadWhole(IDC_CORN_SHRINKAGE, _T("Corn shrinkage"), shrinkage)
		&& Apply(IDC_CORN_SHRINKAGE, _T("Corn"), [&] {
			farm.corn.setEarsPerAcre(ears);
			farm.corn.setKernelsPerEar(kernels);
			farm.corn.setKernelWeight(weight);
			farm.corn.setShrinkage(shrinkage);
		});
}

bool CropsDlg::ReadRice(FarmData& farm)
{
	int panicles = 0;
	int grains = 0;
	double weight = 0;
	return ReadWhole(IDC_RICE_PANICLES, _T("Rice panicles per square metre"), panicles)
		&& ReadWhole(IDC_RICE_GRAINS, _T("Rice grains per panicle"), grains)
		&& ReadNumber(IDC_RICE_GRAIN_WEIGHT, _T("Rice grain weight"), weight)
		&& Apply(IDC_RICE_PANICLES, _T("Rice"), [&] {
			farm.rice.setNumPaniclesPerM2(panicles);
			farm.rice.setGrainsPerPanicle(grains);
			farm.rice.setGrainWeight(weight);
		});
}

bool CropsDlg::ReadForm(FarmData& farm)
{
	m_error.Empty();
	m_errorControl = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (!ReadCommon(CropAt(farm, i), i))
		{
			return false;
		}
	}
	if (!ReadWheat(farm) || !ReadCorn(farm) || !ReadRice(farm))
	{
		return false;
	}

	double fertilizer = 0;
	double water = 0;
	double fertilizerPrice = 0;
	double waterPrice = 0;
	if (!ReadNumber(IDC_FERTILIZER, _T("Fertilizer"), fertilizer)
		|| !ReadNumber(IDC_WATER, _T("Water"), water)
		|| !ReadNumber(IDC_FERTILIZER_PRICE, _T("Fertilizer price"), fertilizerPrice)
		|| !ReadNumber(IDC_WATER_PRICE, _T("Water price"), waterPrice))
	{
		return false;
	}
	farm.harvest.setFertilizer(fertilizer);
	farm.harvest.setWater(water);
	farm.harvest.setPriceFertilizer(fertilizerPrice);
	farm.harvest.setPriceWater(waterPrice);
	return true;
}

bool CropsDlg::Commit(const std::function<bool(FarmData&)>& action)
{
	FarmData updated = GetFarm();
	if (!ReadForm(updated))
	{
		SetDlgItemText(IDC_SUBTITLE, kHint);
		ShowFormError();
		return false;
	}
	ShowStatus(_T(""));
	if (action && !action(updated))
	{
		return false;
	}

	try
	{
		updated.saveCrops();
	}
	catch (const std::exception& e)
	{
		AfxMessageBox(_T("Your changes could not be saved.\n\n") + ToCString(e.what()), MB_ICONERROR);
		return false;
	}

	FarmData& farm = GetFarm();
	farm.wheat = updated.wheat;
	farm.corn = updated.corn;
	farm.rice = updated.rice;
	farm.harvest = updated.harvest;
	LoadControls();
	return true;
}

// Save button and the Enter key
void CropsDlg::OnOK()
{
	if (Commit(nullptr))
	{
		SetDlgItemText(IDC_SUBTITLE, _T("All changes saved."));
	}
}

void CropsDlg::OnCancel()
{
	if (m_dirty)
	{
		int answer = AfxMessageBox(_T("Do you want to save your changes before closing?"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (answer == IDCANCEL || (answer == IDYES && !Commit(nullptr)))
		{
			return;
		}
	}
	ThemedDialog::OnCancel();
}

void CropsDlg::AdvanceStatus(int index)
{
	Commit([index](FarmData& farm) {
		return CropAt(farm, index).advanceStatus();
	});
}

void CropsDlg::OnAdvanceWheat()
{
	AdvanceStatus(0);
}

void CropsDlg::OnAdvanceCorn()
{
	AdvanceStatus(1);
}

void CropsDlg::OnAdvanceRice()
{
	AdvanceStatus(2);
}

void CropsDlg::OpenFieldLog(int index)
{
	Commit([this, index](FarmData& farm) {
		FieldLogDlg dlg(CropAt(farm, index), farm.harvest, this);
		return dlg.DoModal() == IDOK;
	});
}

void CropsDlg::OnFieldLogWheat()
{
	OpenFieldLog(0);
}

void CropsDlg::OnFieldLogCorn()
{
	OpenFieldLog(1);
}

void CropsDlg::OnFieldLogRice()
{
	OpenFieldLog(2);
}

void CropsDlg::OnHarvest()
{
	std::vector<std::string> harvested;
	bool nothingReady = false;
	bool saved = Commit([&](FarmData& farm) {
		harvested = farm.harvest.harvestAndStore(farm.wheat, farm.corn, farm.rice);
		nothingReady = harvested.empty();
		return !nothingReady;
	});
	if (saved)
	{
		AfxMessageBox(_T("Harvested and stored: ") + JoinNames(harvested) + _T("."), MB_ICONINFORMATION);
	}
	else if (nothingReady)
	{
		AfxMessageBox(_T("No crop is ready to harvest.\n\nA crop can be harvested once its growth stage is Maturity."), MB_ICONINFORMATION);
	}
}

void CropsDlg::OnSell()
{
	double earned = 0;
	bool nothingStored = false;
	bool saved = Commit([&](FarmData& farm) {
		const Harvest& harvest = farm.harvest;
		if (harvest.getAmountWheat() + harvest.getAmountCorn() + harvest.getAmountRice() <= 0)
		{
			nothingStored = true;
			return false;
		}
		earned = farm.harvest.sellAndGenerateRevenue(farm.wheat, farm.corn, farm.rice);
		return true;
	});
	if (saved)
	{
		AfxMessageBox(_T("Stored crops sold for Rs. ") + FormatMoney(earned) + _T("."), MB_ICONINFORMATION);
	}
	else if (nothingStored)
	{
		AfxMessageBox(_T("There are no stored crops to sell. Harvest matured crops first."), MB_ICONINFORMATION);
	}
}

void CropsDlg::OnNewSeason()
{
	int answer = AfxMessageBox(
		_T("Start a new season?\n\n")
		_T("This resets the growth stage, field size, quantity, dates and yield inputs of all three crops. ")
		_T("Stored crops, prices and revenue are kept."),
		MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
	if (answer != IDYES)
	{
		return;
	}
	Commit([](FarmData& farm) {
		farm.wheat.startNewSeason();
		farm.corn.startNewSeason();
		farm.rice.startNewSeason();
		return true;
	});
}
