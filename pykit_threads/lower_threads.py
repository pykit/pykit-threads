# -*- coding: utf-8 -*-

"""
Resolve threads operations through external calls to a thread library,
specified in env["library.threads"].
"""

import ctypes

from pykit.parsing import from_c
from pykit.ir import ops
from pykit.lower import RuntimeLowering
from pykit.utils import libraries, call_once

thread_module_source = """
typedef struct Thread;
typedef struct ThreadPool;

extern Thread *thread_start(void *function, void *args); /*:
    { "exc.badval": 0, "exc.raise": "RuntimeError",
      "exc.msg": "Unable to initialize thread" } :*/
extern void thread_join(Thread *thread);

extern ThreadPool *threadpool_start(Int32 nthreads); /*:
    { "exc.badval": 0, "exc.raise": "RuntimeError",
      "exc.msg": "Unable to initialize threadpool" } :*/
extern void threadpool_submit(ThreadPool *threadpool, void *func, void *args);
extern void threadpool_join(ThreadPool *threadpool);
extern void threadpool_close(ThreadPool *threadpool);
"""

thread_module = from_c(thread_module_source, __file__)

def resolve_threading_rt(env, threadmod):
    threadlib = ctypes.CDLL(env["library.threads"])
    libraries.resolve_symbols(threadmod, threadlib)

def run(func, env, threadmod=thread_module):
    """Generate runtime calls into thread library"""
    resolve_threading_rt(env, threadmod)
    func.parent.link(threadmod)
    RuntimeLowering(func).lower_ops_into_runtime(func, ops.oplist("thread_*"))