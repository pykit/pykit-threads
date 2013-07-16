# -*- coding: utf-8 -*-
from __future__ import print_function, division, absolute_import

import sys
from distutils.core import setup, Extension

if sys.version_info[:2] < (2, 6):
    raise Exception('pykit requires Python 2.6 or greater.')

import pykit
from pykit.utils.setup import find_packages, run_2to3, setup_args

exclude_packages = ()
cmdclass = {}

if sys.version_info[0] >= 3:
    run_2to3(cmdclass)

tpprefix = 'pykit_threads/threadpool/'

ext_modules = [
    Extension(
        name = 'cthreading',
        sources = [tpprefix + 'cthreading.c',
                   tpprefix + 'src/threadpool.c',
                   tpprefix + 'src/threading.c'],
        include_dirs = [tpprefix + 'inc'],
    )
]

#===------------------------------------------------------------------===
# setup
#===------------------------------------------------------------------===

setup(
    name="pykit-threads",
    version=pykit.__version__,
    author="Continuum Analytics, Inc.",
    license="BSD",
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2.6",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3.2",
        "Programming Language :: Python :: 3.3",
        "Topic :: Utilities",
        ],
    description="Thread support for pykit",
    packages=find_packages(exclude=exclude_packages),
    package_data={
        '': ['*.md', '*.cfg'],
        },
    ext_modules=ext_modules,
    cmdclass=cmdclass,
    **setup_args
)
