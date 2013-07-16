#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function, division, absolute_import
import sys
import pykit_threads

kwds = {}
if len(sys.argv) > 1:
    kwds["pattern"] = sys.argv[1]

sys.exit(pykit_threads.test())