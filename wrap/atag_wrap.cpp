/////////////////////////////////////////////////////////////////////
//
// Name: atag_wrap.cpp
// Author: Shaojie Wang
// Date: 11/2/2017
// Description: 
//  Wrapper functions for acode detection
//
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include "python2.7/Python.h"
#include "numpy/arrayobject.h"
#include "numpy/ndarraytypes.h"
#include "opencv2/opencv.hpp"

#include "atag_itf.h"

using namespace std;

static PyObject* detect(PyObject *dummy, PyObject *args)
{
    PyObject *arg1=NULL;
    PyArrayObject *arr1=NULL;

    if (!PyArg_ParseTuple(args, "O", &arg1)) {
        return NULL;
    }

    arr1 = (PyArrayObject*)PyArray_FROM_OTF(arg1, NPY_UINT8, NPY_ARRAY_IN_ARRAY);
    if (!arr1) {
        return NULL;
    }

    int nd = PyArray_NDIM(arr1);
    if (nd != 2) {
        Py_DECREF(arr1);
        return Py_BuildValue("[iiddd]", 0, -2, 0.0, 0.0, 0.0);
    }

    npy_intp *the_shape = PyArray_DIMS(arr1);
    int width = the_shape[1];
    int height = the_shape[0];
    //cout << "width = " << width << ", height = " << height << endl;
    unsigned char *mydata = (unsigned char *)PyArray_DATA(arr1);

    int id = -1, myok = 0;
    double o = 0.0, cx = 0.0, cy = 0.0;
    myok = atag_matrix_detect_(width, height, mydata, &id, &o, &cx, &cy);
    Py_DECREF(arr1);
    return Py_BuildValue("[iiddd]", myok, id, o, cx, cy);
}

static PyObject *atag_init(PyObject * self)
{
    atag_init_();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *atag_cleanup(PyObject * self)
{
    atag_cleanup_();
    Py_INCREF(Py_None);
    return Py_None;
}

static struct PyMethodDef methods[] = {
    {"detect", detect, METH_VARARGS, "detect code "},
    {"init", (PyCFunction)atag_init, METH_NOARGS, "Init"},
    {"cleanup", (PyCFunction)atag_cleanup, METH_NOARGS, "Cleanup"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initatag (void)
{
    (void)Py_InitModule("atag", methods);
    import_array();
}

