/*******************************************************************/
/* Name: atag_itf.h                                                */
/*******************************************************************/


#ifndef ATAG_ITF_H
#define ATAG_ITF_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void atag_init_();
EXTERNC void atag_cleanup_();
EXTERNC int atag_matrix_detect_(int width, int height, 
    unsigned char *data, 
    int* id, double* orientation, double* cx, double* cy);

#endif

