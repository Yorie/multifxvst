// HoverButton.cpp : implementation file
//

#include "stdafx.h"
#include "HoverButton.h"
#include ".\hoverbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton

CHoverButton::CHoverButton()
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
  OnOff = false;
  m_value = false;
}

CHoverButton::~CHoverButton()
{
}
IMPLEMENT_DYNAMIC(CHoverButton, CBitmapButton)

BEGIN_MESSAGE_MAP(CHoverButton, CBitmapButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
//  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
 //	CHoverButton message handlers
		
void CHoverButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

BOOL CHoverButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

// Set the tooltip with a string resource
void CHoverButton::SetToolTipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetToolTipText(&sText, bActivate);

}

// Set the tooltip with a CString
void CHoverButton::SetToolTipText(CString *spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CHoverButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
} // End of InitToolTip

// Activate the tooltip
void CHoverButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip






void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	CDC *mydc=CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(mydc);
    CRect r;
	GetClientRect(&r);



	CBitmap * pOldBitmap;
	pOldBitmap = pMemDC -> SelectObject(&mybitmap);
	
	CPoint point(0,0);	
	mydc->SetStretchBltMode(HALFTONE);
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
      mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
		//mydc->StretchBlt(0,0,r.right,r.bottom,pMemDC,m_ButtonSize.cx*2,0,m_ButtonSize.cx,m_ButtonSize.cy,SRCCOPY);

  }
	else
	{
		if(m_bHover)
		{
      if(OnOff)
      {
        if(m_value)
			    mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
        else
			    mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*3,0,SRCCOPY);
      }
      else
      {
			  mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx,0,SRCCOPY);
      }
			//mydc->StretchBlt(0,0,r.right,r.bottom,pMemDC,m_ButtonSize.cx,0,m_ButtonSize.cx,m_ButtonSize.cy,SRCCOPY);
		}else
		{
      if(OnOff)
      {
        if(m_value)
          mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,m_ButtonSize.cx*2,0,SRCCOPY);
        else
          mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
      }
      else
      {
			   mydc->BitBlt(0,0,/*r.right,r.bottom*/m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
      }
    }	
	}
	CString txt;
	GetWindowText(txt);
  if(m_bHover)
    mydc->SetTextColor(RGB(250,10,10));
  else
    mydc->SetTextColor(RGB(0,0,0));

	mydc->SetBkMode(TRANSPARENT );
    mydc->DrawText(txt,CRect(0,0,r.right,r.bottom),DT_VCENTER|DT_CENTER|DT_SINGLELINE  );
	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;
}

// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/Hover
BOOL CHoverButton::LoadBitmap(UINT bitmapid,bool OnOff)
{
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP	bitmapbits;
	mybitmap.GetBitmap(&bitmapbits);
	m_ButtonSize.cy=bitmapbits.bmHeight;
  this->OnOff = OnOff;
  if(OnOff)
    m_ButtonSize.cx=bitmapbits.bmWidth/4;
  else
	  m_ButtonSize.cx=bitmapbits.bmWidth/3;
	//SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE   |SWP_NOOWNERZORDER   );
	return TRUE;
}



LRESULT CHoverButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	Invalidate();
	return 0;
}


LRESULT CHoverButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;

	m_bHover=FALSE;
	Invalidate();
	return 0;
}

//void CHoverButton::OnLButtonDown(UINT nFlags, CPoint point)
//{
//  ::CRect rc;
//  // TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les param�tres par d�faut des appels
//  GetWindowRect(&rc);
//
//  if(rc.PtInRect(point))
//    m_value = ! m_value; 
//  CBitmapButton::OnLButtonDown(nFlags, point);
//}

void CHoverButton::OnLButtonUp(UINT nFlags, CPoint point)
{
  if(OnOff)
  {
    ::CRect rc;
    GetClientRect(&rc);

    if(rc.PtInRect(point))
    {
      m_value = ! m_value; 
      GetParent()->PostMessage(WM_USER + 200,DWORD(m_value),LPARAM(this));
        
    }
  }

  


  CBitmapButton::OnLButtonUp(nFlags, point);
}
