#pragma once
#include "afxdialogex.h"


// CropsDlg dialog

class CropsDlg : public CDialog
{
	CBrush m_backgroundBrush;
	virtual BOOL OnInitDialog();
	DECLARE_DYNAMIC(CropsDlg)

public:
	CropsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CropsDlg();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnprocess();
	afx_msg void OnBnClickedbtnadvancestatuswheat();
	afx_msg void OnDtnDatetimechangedtpwheatplanting(NMHDR* pNMHDR, LRESULT* pResult);
	CDateTimeCtrl SetWheatPlantingDate;
	afx_msg void OnDtnDatetimechangedtpwheatharvesting(NMHDR* pNMHDR, LRESULT* pResult);
	CDateTimeCtrl setWheatHarvestingDate;
	afx_msg void OnBnClickedbtnadvancestatuscorn();
	afx_msg void OnDtnDatetimechangedtpcornplanting(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDtnDatetimechangedtpcornharvesting(NMHDR* pNMHDR, LRESULT* pResult);
	CDateTimeCtrl SetCornPlanting;
	CDateTimeCtrl SetCornHarvesting;
	CDateTimeCtrl SetRicePlanting;
	CDateTimeCtrl SetRiceHarvesting;
	afx_msg void OnBnClickedbtnadvancestatusrice();
	afx_msg void OnDtnDatetimechangedtpriceplanting(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDtnDatetimechangedtpriceharvesting(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedtxtharvestandstore();
	afx_msg void OnBnClickedtxtsellandgeneraterevenue();
	afx_msg void OnBnClickedtxtstartnewseason();
};
