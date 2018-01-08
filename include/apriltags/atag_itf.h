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
EXTERNC bool atag_detect_(cv::Mat& image_gray, int& id, double& orientation, 
            double& cx, double& cy);
EXTERNC bool atag_detect(
    cv::Mat& image_gray,
    int& id,
    std::pair<float, float>& p0,
    std::pair<float, float>& p1,
    std::pair<float, float>& p2,
    std::pair<float, float>& p3
);
#endif

