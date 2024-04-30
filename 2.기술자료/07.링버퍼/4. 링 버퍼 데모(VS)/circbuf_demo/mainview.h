// mainview.h : interface of the CMainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINVIEW_H__8CE12949_54D4_4752_AB40_917CF51C20D1__INCLUDED_)
#define AFX_MAINVIEW_H__8CE12949_54D4_4752_AB40_917CF51C20D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_COLUMNS 3

#define WM_MYMSG (WM_APP+0x123)


#include "CircularBuffer.h"
#include "perftimer.h"

///////////////////////////////////////////////////////////////////////////
// QueInfo
//
// Purpose:		stores info for threads

struct QueInfo
{
	CircularBuffer *	que;			// circular que
	long				index;			// string index
	HANDLE				hWriteEvent;	// handle to write thread done
	HANDLE				hReadEvent;		// handle to read thread done
	bool				retWrite;		// return from write thread
	bool				retRead;		// return from read thread
	string				txtText[3];		// text strings for test
	string				strGet;			// recieves text read
	int					nThrdDone;		// threads reporting completion
	HWND				hWndView;		// window to send custom message to
};

class CMainView : public CListView
{
private:
	int m_nColumnFmt[NUM_COLUMNS];
	int m_nColumnWidth[NUM_COLUMNS];
    int m_nItemCntr;
	TCHAR m_szColumnLabel[NUM_COLUMNS][40];
    CPerfTimer perfTimer;

	QueInfo				*m_pinfo;

    void SetupListView();
    void ClearListView();
    void SetListItem(CString _strIOVal, CString _strData);
    bool  testQue ( QueInfo & info );
    static DWORD __stdcall testWriteQue ( LPVOID lpParameter );
    static DWORD __stdcall testReadQue ( LPVOID lpParameter );


protected: // create from serialization only
	CMainView();
	DECLARE_DYNCREATE(CMainView)

// Attributes
public:
	CMainDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMainView)
	afx_msg LRESULT OnMyMsg(WPARAM wp, LPARAM lp);
	afx_msg void OnViewStart();
	afx_msg void OnUpdateViewStart(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mainview.cpp
inline CMainDoc* CMainView::GetDocument()
   { return (CMainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEW_H__8CE12949_54D4_4752_AB40_917CF51C20D1__INCLUDED_)
