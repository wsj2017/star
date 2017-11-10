/*******************************************************************/
/* Name: atag_wrap.h                                               */
/*******************************************************************/


#ifndef ATAG_WRAP_H
#define ATAG_WRAP_H

/* Required before using atag */
void atag_init();

/* Required after using atag */
void atag_cleanup();

/* */
int atag_detect(int width, int height, 
        unsigned char *data, int *id, double* orientation, 
        double* cx, double* cy);

#endif
