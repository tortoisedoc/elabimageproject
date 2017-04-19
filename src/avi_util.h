
#include "cv\highgui.h"

typedef struct {
		char * file_name;
		IplImage* frame;
} AVI_READER;	



int open_avi(AVI_READER *);
char * get_next_frame();
int display_image(int delay);
int close_avi();













