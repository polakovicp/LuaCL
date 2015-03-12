#include "pphive.h"

PyObject * get_module(const char * name)
{
	PyObject * modDict = PyImport_GetModuleDict();

	if(modDict)
	{
		PyObject * ioMod = PyDict_GetItemString(modDict, name);

		if(ioMod)
			return ioMod;
	}

	return NULL;
}
///
/// Returns new reference PyBytesObject containing all bytes from file
///
PyObject * read_file(const char * name)
{
	PyObject * ioMod = get_module("io");

	if(ioMod)
	{
		PyObject * fdObj = PyObject_CallMethod(ioMod, "open", "ss", name, "rb");

		if(fdObj)
		{
			PyObject * bytesObj = (PyObject *)PyObject_CallMethod(fdObj, "read", NULL);

			if(!bytesObj)
			{
				Py_DECREF(fdObj);
				return NULL;
			}

			PyObject * _Obj = (PyObject *)PyObject_CallMethod(fdObj, "close", NULL);

			Py_XDECREF(_Obj);
			Py_DECREF(fdObj);

			return bytesObj;
		}
	}

	return NULL;
}

static PyMethodDef PPHiveMethods[] =
{
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef PPHiveModule = 
{
	PyModuleDef_HEAD_INIT, "pphive", NULL, -1, PPHiveMethods
};

inline bool init_type(PyTypeObject * o)
{
	if(PyType_Ready(o) < 0)
		return false;

	return true;
}

inline bool add_type(PyObject * m, PyTypeObject * o, const char * n)
{
	Py_INCREF(o);

	if(PyModule_AddObject(m, n, (PyObject *)o) == -1)
		return false;

	return true;
}

PyMODINIT_FUNC PyInit_pphive(void)
{
    PyObject * HiveError = NULL;

    PyObject * module;

    if(!init_type(&PyHive_Type))
        return NULL;

    module = PyModule_Create(&PPHiveModule);

    if (module == NULL || 
        !add_type(module, &PyHive_Type, "Hive"))
    {
        return NULL;
    }

    // init exception
    HiveError = PyErr_NewException("pphive.HiveError", PyExc_Exception, NULL);

    if(!HiveError)
        return NULL;

    Py_INCREF(HiveError);

    if(PyModule_AddObject(module, "HiveError", HiveError) == -1)
        return NULL;

    return module;
}
