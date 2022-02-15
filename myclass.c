#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

#include "myclass.h"

typedef struct {
    PyObject_HEAD
    /* class members */
    int      number;
    PyObject *object;

    /* internal / special stuff */
    void     *memory;
    PyObject *hidden;
    PyObject *weaklist;
} myclass_object;

static int myclass_traverse(myclass_object *self, visitproc visit, void *arg)
{
    Py_VISIT(self->hidden);
    Py_VISIT(self->object);

    return 0;
}

static int myclass_clear(myclass_object *self)
{
    /* delete py objects */
    Py_CLEAR(self->hidden);
    Py_CLEAR(self->object);

    /* delete c-objects */
    PyMem_Free(self->memory);

    return 0;
}

static void myclass_dealloc(myclass_object *self)
{
    PyObject_GC_UnTrack(self);

    /* delete dynamic allocations */
    PyObject_ClearWeakRefs((PyObject*)self);
    myclass_clear(self);

    /* delete class instance */
    Py_TYPE(self)->tp_free(self);
    Py_DECREF(Py_TYPE(self));
}

static PyObject* myclass_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    myclass_object *self;

    if ((self = (myclass_object*)type->tp_alloc(type, 0)) == NULL)
        return NULL;

    if (!PyArg_ParseTuple(args, "|U:__new__", &self->hidden))
        return NULL;

    Py_XINCREF(self->hidden);
    self->memory = PyMem_Malloc(32);

    return (PyObject*)self;
}

static PyObject* myclass_repr(myclass_object *self)
{
    if (self->hidden != NULL) {
        Py_INCREF(self->hidden);
        return self->hidden;
    }

    return PyUnicode_FromFormat("MyClass_%p", self);
}

static PyObject* myclass_add(myclass_object *self, PyObject *args)
{
    double a, b;

    if (!PyArg_ParseTuple(args, "dd:add", &a, &b))
        return NULL;

    return PyFloat_FromDouble(a + b);
}

static PyObject* myclass_cat(myclass_object *self, PyObject *args)
{
    PyObject *first, *second;

    if (!PyArg_ParseTuple(args, "UU:concat", &first, &second))
        return NULL;

    return PyUnicode_Concat(first, second);
}

/******************************************************************************/
/* myclass myclass object definition                                          */
/******************************************************************************/

static PyMemberDef myclass_members[] = {
        {"a_number", T_INT,    offsetof(myclass_object, number), 0},
        {"a_object", T_OBJECT, offsetof(myclass_object, object), 0},

/* ... more members ... */

        /* special members */
        {"__weaklistoffset__", T_PYSSIZET, offsetof(myclass_object, weaklist), READONLY},

        { NULL }
};

static PyMethodDef myclass_methods[] = {
        { "add", (PyCFunction)myclass_add, METH_VARARGS,  "" },
        { "cat", (PyCFunction)myclass_cat, METH_VARARGS,  "" },

/* ... more methods ... */

        { NULL }
};

static PyType_Slot myclass_slots[] = {
        { Py_tp_doc,      PyDoc_STR("MyClass example") },
        { Py_tp_repr,     myclass_repr },
        { Py_tp_new,      myclass_new },
        { Py_tp_traverse, myclass_traverse },
        { Py_tp_clear,    myclass_clear },
        { Py_tp_dealloc,  myclass_dealloc },
        { Py_tp_members,  myclass_members },
        { Py_tp_methods,  myclass_methods },
        { 0, NULL }
};

/* myclass heap type definition */
static PyType_Spec myclass_spec = {
        .name      = "MyClass",
        .basicsize = sizeof(myclass_object),
        .itemsize  = 0,
        .flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
        .slots     = myclass_slots
};

/* new myclass heap object */
PyObject* myclassobj_new()
{
    PyTypeObject *sub_tp;

    /* create the heap object */
    sub_tp = (PyTypeObject*)PyType_FromSpecWithBases(&myclass_spec, NULL);
    PyType_Ready(sub_tp);

    return (PyObject*)sub_tp;
}