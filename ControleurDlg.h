#pragma once

#include "resource.h"
#include "skin\skinlistctrl.h"
#include "afxwin.h"


// Bo�te de dialogue CControleurDlg
class CAppPointer;
class CControleurDlg : public CDialog
{
	DECLARE_DYNAMIC(CControleurDlg)
  void SetAPP(CAppPointer * m_APP);
public:
	CControleurDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CControleurDlg();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CONTROLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
  CAppPointer * APP;
public:
  ::CFont font;
  void Update();
  virtual BOOL OnInitDialog();
  CSkinListCtrl m_lstcontroleur;
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

};
