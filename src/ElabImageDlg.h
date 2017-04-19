// ElabImageDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "cv\cv.h"
#include "cv\highgui.h"


#define N_MAX_FUN 100
#define N_BG_INIT_LIMIT 60 //NR FRAME PER INIZIALIZZAZIONE BG0
#define N_CHANGED 255  //VALORE CHANGED
#define N_UNCHANGED 0  //VALORE UNCHANGED
#define N_CHANGE_THRESHOLD 10  //SOGLIA PER LA DIFFERENZIAZIONE CHANGED / UNCHANGED
#define N_FRAMEWIDTH 320
#define N_FRAMEHEIGHT 240
#define N_COLORDEPTH 256

//Struttura per la gestione del Background
typedef unsigned char TBackgroundStruct [N_FRAMEWIDTH][N_FRAMEHEIGHT][N_COLORDEPTH+1];

//struct TBackgroundStruct{
	  //int data [320][240][257];
//};

// CElabImageDlg dialog
class CElabImageDlg : public CDialog
{
// Construction
public:
	CElabImageDlg(CWnd* pParent = NULL);	// standard constructor

// Destruction
	~CElabImageDlg();
// Dialog Data
	enum { IDD = IDD_ELABIMAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	void Init(IplImage* inputImage);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFile();
	CComboBox m_combo_box;
	CString m_fname;
	CButton m_btnSave;
	CButton m_btnElab;
	CString* (m_functions[N_MAX_FUN]);
	void ReleaseAll(void);
	int getIndexFun(CString& s);
	TBackgroundStruct * background;
	void myfun(IplImage* inputImage);
	//Adaptive Background functions
	void AdaptiveBackground01(IplImage* inputImage);
	void AdaptiveBackground02(IplImage* inputImage);
	void updateBackGround (IplImage* inputImage);
	void computeChangeMask (IplImage* inputImage);
	void myfun_release();
	IplImage* previous_frame,*currentImageGray,*frame_diff,*first_frame, *changeMask;
	int frame_number;
	CString winOut;
};
