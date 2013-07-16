from distutils.core import setup, Extension

ext_modules = [
    Extension(
        name = 'cthreading',
        sources = ['cthreading.c', 'src/threadpool.c', 'src/threading.c'],
        include_dirs = ['inc'],
    )
]

setup(
    name = 'cthreading',
    author = 'Continuum Analytics, Inc.',
    ext_modules = ext_modules,
)
