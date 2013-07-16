import cthreading
from ctypes import *

cthreading.init(10, 4)

libc = CDLL("libc.dylib") 
cfunc = libc.puts

keepalive = []
for i in range(1000000):
    carg = c_char_p("%d" % i)
    keepalive.append(carg)
    cthreading.enqueue(cast(cfunc, c_void_p).value,
                       cast(carg, c_void_p).value)

print 'joinning'
cthreading.join()

