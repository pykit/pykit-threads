# -*- coding: utf-8 -*-

"""
Install the thread runtime library and lowering pass.
"""

from __future__ import print_function, division, absolute_import
from os.path import join, abspath, dirname

from pykit_threads import lower_threads

root = abspath(dirname(__file__))

def install(env):
    env["passes.lower_threads"] = lower_threads
    env["library.threads"] = join(root, "libthreads")