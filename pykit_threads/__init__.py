from os.path import dirname, abspath
import pykit

test = lambda: pykit.test(dirname(dirname(abspath(__file__))))