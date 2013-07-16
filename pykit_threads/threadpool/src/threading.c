#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN32__) 
    #include "thread_windows.c"
#else
    #include "thread_posix.c"
#endif
