#include "pphive.h"

///
///
///
static PyObject * expand(PyHive * self, PyObject * args)
{
    return PyErr_Format(HiveError, "test");
};
///
///
///
static PyObject * PyHive_new(PyTypeObject * type, PyObject * args, PyObject * kwds)
{
	PyHive * self = (PyHive *)type->tp_alloc(type, 0);

	if(self)
	{
		self->base = PyBytes_FromString("");

		if(!self->base)
		{
			Py_DECREF(self);
			return NULL;
		}
	}

	return (PyObject *)self;
}
///
///
///
static int PyHive_init(PyHive * self, PyObject * args, PyObject * kwds)
{
	const char * path;

	if(!PyArg_ParseTuple(args, "s", &path))
		goto ERR;

	PyObject * bytesObj = read_file(path);

	if(!bytesObj || !PyBytes_Check(bytesObj))
		goto ERR_BAD_FILE;

	self->base = (PyObject *)bytesObj;
	return 0;

ERR_BAD_FILE:
	Py_XDECREF(bytesObj);
ERR:
	return -1;
}

static void PyHive_dealloc(PyHive * self)
{
    Py_XDECREF(self->base);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyMemberDef PyHive_members[] =
{
    {NULL}
};

static PyMethodDef PyHive_methods[] =
{
    {"expand", (PyCFunction)expand, METH_VARARGS, "Expands hive"},
    {NULL}
};

static PyTypeObject PyHive_Type = 
{
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "acreged.Hive", 
    sizeof(PyHive), 
    0,
    (destructor)PyHive_dealloc, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0,
    0, 
    0, 
    0, 
    0, 
    Py_TPFLAGS_DEFAULT,
    "Hive object",
    0, 
    0, 
    0, 
    0, 
    0, 
    0,
    (PyMethodDef *)PyHive_methods, 
    (PyMemberDef *)PyHive_members,
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    (initproc)PyHive_init, 
    0, 
    PyHive_new,
};