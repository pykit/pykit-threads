# -*- coding: utf-8 -*-
from __future__ import print_function, division, absolute_import

import unittest

from pykit.environment import fresh_env
from pykit.parsing import from_c
from pykit.transform import ret
from pykit.utils import opcodes
from pykit.types import Pointer, Opaque

testfunc = """
void compute(void *args) {

}

int func(int i) {
    int x = 0;
    Opaque *thread = thread_start();
}
"""

class TestThreads(unittest.TestCase):
    def test_threads(self):
        mod = from_c(testfunc)
        func = mod.get_function("func")
        ret.run(func, fresh_env())
        ops = opcodes(func)
        self.assertEqual(ops.count("ret"), 1, ops)