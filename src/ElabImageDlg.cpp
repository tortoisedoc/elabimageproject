// ElabImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElabImage.h"
#include "ElabImageDlg.h"
#include "avi_util.h"
#include "cv\highgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CElabImageDlg dialog


//Constructor for initialization
CElabImageDlg::CElabImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CElabImageDlg::IDD, pParent)
	, m_fname(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//Init background structure
	background = (TBackgroundStruct *) malloc (sizeof(TBackgroundStruct));
	memset (background,0,sizeof(TBackgroundStruct));
}

void CElabImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo_box);
	DDX_Control(pDX, IDOK, m_btnElab);
}

BEGIN_MESSAGE_MAP(CElabImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_FILE, OnBnClickedFile)
END_MESSAGE_MAP()


// CElabImageDlg message handlers


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CElabImageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CElabImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CElabImageDlg::OnBnClickedCancel()
{
	// Add your control notification handler code here

}

void CElabImageDlg::OnBnClickedFile()
{

	static char BASED_CODE szFilter[] = "AVI Files (*.avi)|*.avi|";
	CFileDialog	fdlg(TRUE, NULL, NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			szFilter,this,sizeof(OPENFILENAME ));
	int res=fdlg.DoModal();
	if(res==1){
		m_fname=fdlg.GetPathName();
		m_btnElab.EnableWindow();
	}

}
void CElabImageDlg::OnBnClickedButton1()
{

}

void CElabImageDlg::ReleaseAll(void)
{

	int i=0;
	while(m_functions[i]!=0 && i<N_MAX_FUN){
		delete m_functions[i];
		m_functions[i]=0;
		i++;
	}
}

int CElabImageDlg::getIndexFun(CString& s)
{
	int i=0;
	while(m_functions[i]!=0 && i<N_MAX_FUN){
		if(s==*(m_functions[i])) return i;
		i++;
	}
	return -1;
}


BOOL CElabImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Add extra initialization here
	m_fname.Empty();
	int i;
	for(i=0;i<N_MAX_FUN;i++)m_functions[i]=0;

	m_functions[0]=new CString("two frame difference");
	m_functions[1]=new CString("Adaptive Background Ver 0.1");
	m_functions[2]=new CString("Adaptive Background & Frame Difference Ver 0.1");

	i=0;
	while(m_functions[i]!=0&& i<N_MAX_FUN){
		m_combo_box.AddString(*(m_functions[i]));
		i++;
	}
	m_combo_box.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CElabImageDlg::OnBnClickedOk()
{
	CString function;
	int cursel=m_combo_box.GetCurSel();
	if(cursel!=-1)m_combo_box.GetLBText(cursel,function); 
	else {
		AfxMessageBox("selezionare una funzione dalla lista!");
		return;
	}
	AVI_READER avi;
	avi.file_name=(char*)m_fname.GetString();
	char* im=0;
	frame_number=0;
	char retcode=-1;
	open_avi(&avi);

	while((im=get_next_frame())!=0 && retcode!='q')
	{
		switch(getIndexFun(function)){
			case 0:	//two frame difference
				myfun(avi.frame);
				break;
			case 1://project function - background updating, background subtraction
				AdaptiveBackground01 (avi.frame);
			break;
			case 2:
				AdaptiveBackground02 (avi.frame);
			break;
			default:
				AfxMessageBox("errore nella ricerca della funzione!");
		}
		retcode=(char)(display_image(500));
		frame_number++;
	}
	
	close_avi();
	switch(getIndexFun(function)){
		case 0:
				myfun_release();
				break;
		default:
				AfxMessageBox("errore nella ricerca della funzione!");
	}
	ReleaseAll();
	OnCancel();
}

//Inizializzazione - Immagine usata solo per reperire larghezza ed altezza
void CElabImageDlg::Init(IplImage* inputImage)
{
	CvSize in_size;
	in_size.height=inputImage->height;
	in_size.width=inputImage->width;	
	changeMask=cvCreateImage(in_size,IPL_DEPTH_8U,1);
	currentImageGray=cvCreateImage(in_size,IPL_DEPTH_8U,1);
	winOut="Background Subtraction with Dynamical Background Updating";
	cvNamedWindow(winOut, 0);
}


//Funzione per l'aggiornamento del Background
void CElabImageDlg::updateBackGround (IplImage* inputImage)
{
	//Actualize background
	int x,y,tem=0;
	for (y=0;y<inputImage->height;y++)
	{
		for (x=0;x<inputImage->width;x++)
		{
			//Aggiorna frequenza valore pixel
			tem =  (*background)[x][y][inputImage->imageData[x+ (y*inputImage->width)]];
			(*background)[x][y][inputImage->imageData[x+ (y*inputImage->width)]] = tem +1;
//			background[x][y][inputImage->imageData[x+ (y*inputImage->width)]]++;
			//Controlla se il valore ha freq max
			if ((*background)[x][y][inputImage->imageData[x+ (y*inputImage->width)]]>(*background)[x][y][(*background)[x][y][0]])
				//Aggiorna valore pixel con freq max
				(*background)[x][y][0]=inputImage->imageData[x+ (y*inputImage->width)];
		}
	}
}

void CElabImageDlg::computeChangeMask (IplImage* inputImage)
{
	int x,y;
	for (y=0;y<inputImage->height;y++)
	{
		for (x=0;x<inputImage->width;x++)
		{
			if (abs(inputImage->imageData[x+ (y*inputImage->width)] - (*background)[x][y][0])<N_CHANGE_THRESHOLD)
				changeMask->imageData[x+ (y*inputImage->width)] = N_UNCHANGED;
			else changeMask->imageData[x+ (y*inputImage->width)] = N_CHANGED;
		}
	}
}

//Background initialization con semplice somma "blind" sulla serie temporale dei valori
void CElabImageDlg::AdaptiveBackground01(IplImage* inputImage)
{
	if (frame_number==0) Init (inputImage);
	cvCvtColor(inputImage,currentImageGray,CV_BGR2GRAY);
	if (frame_number<N_BG_INIT_LIMIT)updateBackGround (currentImageGray);
	if (frame_number>0) computeChangeMask(currentImageGray);
	cvResizeWindow(winOut,changeMask->width,changeMask->height);
	cvShowImage(winOut, changeMask);
}

//Background initialization con somma "selctive"
void CElabImageDlg::AdaptiveBackground02(IplImage* inputImage)
{
	if (frame_number==0)
	{
		Init (inputImage);
//		previous_frame=cvCreateImage(in_size,IPL_DEPTH_8U,1);
	}

}


void CElabImageDlg::myfun(IplImage* inputImage)
{
	double  thresh=13.0;
	if(frame_number==0){
		CvSize in_size;
		in_size.height=inputImage->height;
		in_size.width=inputImage->width;
		previous_frame=cvCreateImage(in_size,IPL_DEPTH_8U,1);
		currentImageGray=cvCreateImage(in_size,IPL_DEPTH_8U,1);
		frame_diff=cvCreateImage(in_size,IPL_DEPTH_8U,1);
		winOut="two frame difference";
		cvNamedWindow(winOut, 0);
	}

	cvCvtColor(inputImage,currentImageGray,CV_BGR2GRAY);
	if(frame_number==0){//swap
		IplImage *temp=previous_frame;
		previous_frame=currentImageGray;
		currentImageGray=temp;
		return;
	}
	cvAbsDiff(currentImageGray,previous_frame,frame_diff);
	cvThreshold( frame_diff,frame_diff,thresh,255, CV_THRESH_BINARY );

	cvErode(frame_diff,frame_diff,NULL,1);
	cvDilate(frame_diff,frame_diff,NULL,1);
	
	
//VISUALIZZO
	cvResizeWindow(winOut,frame_diff->width,frame_diff->height);
	cvShowImage(winOut, frame_diff);
}

void CElabImageDlg::myfun_release(){
		cvReleaseImage(&previous_frame);
		cvReleaseImage(&currentImageGray);
		cvReleaseImage(&frame_diff);
		cvDestroyWindow(winOut);
}

//Distruttore
CElabImageDlg::~CElabImageDlg(void){
	cvReleaseImage(&changeMask);
	cvDestroyWindow(winOut);
	free(background);
}

