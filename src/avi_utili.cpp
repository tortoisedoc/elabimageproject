#include "avi_util.h"


CvCapture* cap=0;
AVI_READER * avi_struct=0;
IplImage* ipl=0;
char *win=0;
IplImage* copyIpl=0;

int open_avi(AVI_READER * avi_h){
	avi_struct=avi_h;
	cap= cvCaptureFromAVI( avi_h->file_name);
	if(cap==0) return -1;
	win=avi_h->file_name;
	cvNamedWindow(win, 0);
	cvGrabFrame( cap );
	ipl= cvRetrieveFrame( cap );
	avi_struct->frame=ipl;
	if(ipl->nChannels!=3) {
		cvReleaseCapture( &cap );
		return -2;
	}
	copyIpl=cvCloneImage(ipl);
	cvFlip( copyIpl);
	copyIpl->origin=0;
	avi_struct->frame= copyIpl;
	return 1;
}

char * get_next_frame(){
	int res=0;
	char* imag=0;
	if(ipl==0) {
		res= cvGrabFrame( cap );
		if(res>0){
			ipl= cvRetrieveFrame( cap );
			cvFlip(ipl,copyIpl);
			imag=copyIpl->imageData;
		}
	}
	else{
		imag=copyIpl->imageData;
	}
	ipl=0;
	return imag;
}

int display_image(int delay){
	cvResizeWindow(win,copyIpl->width,copyIpl->height);
	cvShowImage(win, copyIpl);
	return cvWaitKey(delay);
}


int close_avi(){
	if(cap!=0) cvReleaseCapture( &cap );
	if(copyIpl!=0)cvReleaseImage(&copyIpl);
	if(win!=0){
		cvDestroyWindow(win);
		win=0;
	}
	return 1;
}


