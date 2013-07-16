import sys, os.path
import subprocess

path = os.path.abspath(sys.argv[1])
for i in xrange(1000):
    subprocess.check_call([path])
