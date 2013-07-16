#include <Python.h>
#include <threading/threadpool.h>

/* a process-wide thread pool */
static ThreadPool threadpool;

static int initialized_flag = 0;

static
PyObject*
init(PyObject *self, PyObject *args)
{
    int capacity, numthreads;
    if ( !PyArg_ParseTuple(args, "ii", &capacity, &numthreads) ) {
        return NULL;
    }
    if ( ThreadPool_init(&threadpool, capacity, numthreads) ) {
        PyErr_SetString(PyExc_Exception, "can't initialize thread pool");
        return NULL;
    }
    initialized_flag = 1;
    Py_RETURN_NONE;
}

static
PyObject*
is_initialized(PyObject *self, PyObject *args)
{
    if ( ! PyArg_ParseTuple(args, "") ) {
        return NULL;
    }
    return Py_BuildValue("i", initialized_flag);
}

static
PyObject*
enqueue(PyObject *self, PyObject *args)
{
    Py_ssize_t callfunc, callargs;
    if ( !PyArg_ParseTuple(args, "nn", &callfunc, &callargs) ) {
        return NULL;
    }
    ThreadPool_enqueue(&threadpool, (void(*)(void*))callfunc, (void*)callargs);
    Py_RETURN_NONE;
}

static
PyObject*
join(PyObject *self, PyObject *args)
{
    if ( ! PyArg_ParseTuple(args, "") ) {
        return NULL;
    }
    ThreadPool_join(&threadpool);
    Py_RETURN_NONE;
}

static PyMethodDef core_methods[] = {
#define declmethod(func) { #func , ( PyCFunction )func , METH_VARARGS , NULL }
    declmethod(init),
    declmethod(is_initialized),
    declmethod(enqueue),
    declmethod(join),
    { NULL },
#undef declmethod
};

/* Module main function, hairy because of py3k port */

#if (PY_MAJOR_VERSION >= 3)
    struct PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "cthreading",
        NULL,
        -1,
        core_methods,
        NULL, NULL, NULL, NULL
    };
#define INITERROR return NULL
    PyObject *
    PyInit_cthreading(void)
#else
#define INITERROR return
    PyMODINIT_FUNC
    initcthreading(void)
#endif
    {
#if PY_MAJOR_VERSION >= 3
        PyObject *module = PyModule_Create( &module_def );
#else
        PyObject *module = Py_InitModule("cthreading", core_methods);
#endif
        if (module == NULL)
            INITERROR;
#if PY_MAJOR_VERSION >= 3

        return module;
#endif
    }

