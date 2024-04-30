// mainview.cpp : implementation of the CMainView class
//

#include "stdafx.h"
#include "circbuf.h"

#include "maindoc.h"
#include "mainview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainView* g_pMainView= NULL;

/////////////////////////////////////////////////////////////////////////////
// CMainView

IMPLEMENT_DYNCREATE(CMainView, CListView)

BEGIN_MESSAGE_MAP(CMainView, CListView)
	//{{AFX_MSG_MAP(CMainView)
	ON_COMMAND(ID_VIEW_START, OnViewStart)
	ON_UPDATE_COMMAND_UI(ID_VIEW_START, OnUpdateViewStart)
	ON_MESSAGE(WM_MYMSG, OnMyMsg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainView construction/destruction

CMainView::CMainView()
{
	// TODO: add construction code here
   m_nItemCntr=0;
   m_pinfo = NULL;
}

CMainView::~CMainView()
{
}

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

void CMainView::ClearListView() 
{
	// TODO: Add your control notification handler code here
	CListCtrl& ListCtrl = GetListCtrl();

	ASSERT_VALID(this);

    ListCtrl.DeleteAllItems();
    m_nItemCntr =0;
}

void CMainView::SetupListView()
{
	strcpy(m_szColumnLabel[0], _T("R/W"));
	strcpy(m_szColumnLabel[1], _T("String"));
	strcpy(m_szColumnLabel[2], _T("Length"));

	m_nColumnFmt[0] = LVCFMT_LEFT;
	m_nColumnFmt[1] = LVCFMT_LEFT;
	m_nColumnFmt[2] = LVCFMT_RIGHT;

	m_nColumnWidth[0] = 60;
	m_nColumnWidth[1] = 330;
	m_nColumnWidth[2] = 60;
}

void CMainView::SetListItem(CString _strIOVal, CString _strData)
{
// insert items
    int nItem = m_nItemCntr++;
    int nLen=0;
    CString strTemp(_T(""));
	LV_ITEM lvi;

//	ASSERT_VALID(this);

	CListCtrl& ListCtrl = GetListCtrl();

	_strIOVal.TrimLeft();

	if (!_strIOVal.IsEmpty())
	{
	 nLen = _strData.GetLength();
	 strTemp.Format("%d",nLen-2);
	}

    ZeroMemory(&lvi,sizeof(lvi));

	lvi.mask = LVIF_TEXT;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
 	lvi.pszText = (LPSTR)_strIOVal.GetBuffer(1);

	ListCtrl.InsertItem(&lvi);

	ListCtrl.SetItemText(nItem,1,_strData);
	ListCtrl.SetItemText(nItem,2,strTemp);

	ListCtrl.SetRedraw(TRUE);

}

/////////////////////////////////////////////////////////////////////////////
// CMainView drawing

void CMainView::OnDraw(CDC* pDC)
{
	CMainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CMainView::OnInitialUpdate()
{
	LV_COLUMN lvc;
	int i=0;

    // TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	CListView::OnInitialUpdate();

	SetupListView();

    CListCtrl& ListCtrl = GetListCtrl();

	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_FMT;

	for(i = 0; i< NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = m_szColumnLabel[i];
		lvc.cx = m_nColumnWidth[i];
		lvc.fmt = m_nColumnFmt[i];
		ListCtrl.InsertColumn(i,&lvc);
	}

	SetListItem(" ","Select the View | Start menu options");

	g_pMainView = this;

}

/////////////////////////////////////////////////////////////////////////////
// CMainView printing

BOOL CMainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CListView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMainDoc* CMainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////
// testWriteQue
//
// Purpose:		thread to write from circular que

DWORD __stdcall CMainView::testWriteQue ( LPVOID lpParameter )
{
	// get params
	QueInfo &	    info  = *( (QueInfo *) lpParameter );
	CircularBuffer &	que	  = *info.que;
	long			index = info.index;
	long count = 0l;
    CString strTemp(_T(""));

	// set current text put and txt end
	const char * txt = info.txtText[index].c_str();
	long length      = info.txtText[index].size();

	char * txtPut = (char *) txt;
	char * txtEnd = (char *) txt + length;
	char temp[1024];

	// show not finished writing string
	bool putDone = false;

    CMainView *pThisObject = static_cast< CMainView*>(g_pMainView);

	// perform write
	while ( !putDone )
	{
		if ( que.writeCount(count) )
		{
			// write min of text remaing to 
			// write or max can put into buffer
			count = min(strlen(txtPut),count);

			if ( count > 0 )
			{
				que.write( (BYTE *) txtPut, count );

				strncpy(temp+1, txtPut, count);
				temp[0] = '"';
				temp[count+1] = '"';
				temp[count+2] = NULL;
				pThisObject->SetListItem("W",temp);

				txtPut += count;

				if ( txtPut == txtEnd )
					putDone = true;
			}
		}
	}

	// get if success
	info.retWrite = putDone;

	// show done
   ::PostMessage(info.hWndView, WM_MYMSG, 1, (LPARAM)(&info));

	return putDone;
}

///////////////////////////////////////////////////////////////////////////
// testReadQue
//
// Purpose:		thread to read from circular que

DWORD __stdcall CMainView::testReadQue ( LPVOID lpParameter )
{
	// buffer for gets
	char txtGet[256];

	// get params
	QueInfo &	    info   = *( (QueInfo *) lpParameter );
	CircularBuffer &	que	   = *info.que;
	long			index  = info.index;
	string &		strGet = info.strGet;
	long count = 0l;
	char temp[1024];

	// string for collecting text
	strGet = "";

	// show not finished reading string
	bool getDone = false;
    CString strTemp(_T(""));

    CMainView *pThisObject = static_cast< CMainView*>(g_pMainView);

	// perform read
	while ( !getDone )
	{
		if ( que.readCount(count) )
		{
			// read min of text remaing to 
			// read or max can get from buffer
			count = min( info.txtText[index].size() - info.strGet.size(),count);

			// get text
			memset(txtGet,0,256);
			que.read( (BYTE *) txtGet, count );

			strGet += txtGet;
			if ( strGet.size() >= info.txtText[index].size() )
				getDone = true;

     		strncpy(temp+1, txtGet, count);
			temp[0] = '"';
			temp[count+1] = '"';
			temp[count+2] = NULL;
			pThisObject->SetListItem("R",temp);
		}
	}

	// get if success
	info.retRead = getDone;

	// show done
	::PostMessage(info.hWndView, WM_MYMSG, 0, (LPARAM)(&info));

	return getDone;

}

///////////////////////////////////////////////////////////////////////////
// testQue
//
// Purpose:		text read/write to que

bool CMainView::testQue ( QueInfo & info )
{
	perfTimer.Start(true);  //reset timer each test

	// init thread handles
	HANDLE hWriteThread = 0;
	HANDLE hReadThread  = 0;

	// init thread ids
	ULONG idWriteThread = 0;
	ULONG idReadThread  = 0;

	// init return values from values
	info.retWrite = false;	
	info.retRead  = false;	

	// initialize number of threads completing the work
	info.nThrdDone = 0;

	// create read thread
	hReadThread = 
	CreateThread( NULL, 0, testReadQue, (LPVOID) &info, 0, &idReadThread );

	// create write thread
	hWriteThread = 
	CreateThread( NULL, 0, testWriteQue, (LPVOID) &info, 0, &idWriteThread );

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CMainView message handlers

void CMainView::OnViewStart() 
{
	long queSz = 50;

	ClearListView();

	// create que
	CircularBuffer* pque = new CircularBuffer;		// allocate queue on heap
	pque->create(queSz);

	// init thread info
	m_pinfo = new QueInfo;							// allocate QueInfo on heap
	m_pinfo->que   = pque;
	m_pinfo->index = 0;

	// initialize text strings
	m_pinfo->txtText[0] = "This is a long line of text. Using it I can test the circular que. Obviously, it is a pointless piece of text. So what. Take a cigar and shove it.";
	m_pinfo->txtText[1] = "Could there be anything worse than typing in pointless text for testing?";
	m_pinfo->txtText[2] = "Yes there could be. Typing in the answer. Way to many nights with the computer!";

	m_pinfo->hWndView = m_hWnd;	// send notification to current view window

	// start test que
	testQue(*m_pinfo);
}


LRESULT CMainView::OnMyMsg(WPARAM wp, LPARAM lp)
{
	CString strTemp(_T(""));

	m_pinfo->nThrdDone++;

	if (m_pinfo->nThrdDone == 2)			// Both threads terminated!
	{
     	perfTimer.Stop();
 
		if (m_pinfo->retRead && m_pinfo->retWrite && m_pinfo->strGet == m_pinfo->txtText[m_pinfo->index])
          strTemp.Format(">>>>Test #%d: Passed, Time %5.2f secs, Queue size %ld",m_pinfo->index+1,perfTimer.Elapsed(),m_pinfo->que->size());
		else
		  strTemp.Format(">>>>Test #%d: Failed, Time %5.2f secs, Queue size %ld",m_pinfo->index+1,perfTimer.Elapsed(),m_pinfo->que->size());
	
		SetListItem(" ",strTemp);
	
		strTemp.Empty();
		SetListItem(" ",strTemp);

		if (++m_pinfo->index < 3)
		{
			testQue(*m_pinfo);
		}
		else
		{
			delete m_pinfo->que;
			delete m_pinfo;
			m_pinfo = NULL;
	        AfxMessageBox("All Done");
		}
	}


	return 0;
}

void CMainView::OnUpdateViewStart(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_pinfo == NULL);
}
