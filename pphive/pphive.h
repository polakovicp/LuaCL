
#ifndef Py_PPHIVE_H
#define Py_PPHIVE_H

#ifdef _DEBUG
#define _DEBUG_BUILD 1
#undef _DEBUG
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

    #include <Python.h>
    #include <structmember.h>

    extern PyObject * HiveError;

#ifdef __cplusplus
}
#endif

#ifdef _DEBUG_BUILD
#define _DEBUG 1
#endif

#include <cstdlib>
#include <fstream>

#ifdef __cplusplus
extern "C" 
{
#endif
    typedef struct 
    {
        PyObject_HEAD
        PyObject *      base;
    } PyHive;

    extern __declspec(dllexport) PyTypeObject PyHive_Type;

    PyObject * read_file(const char * name);
    PyObject * get_module(const char * name);

#ifdef __cplusplus
}
#endif

#endif
