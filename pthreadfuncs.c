#include "pthreadfuncs.h"

int g_fd = -1;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

pid_t getThreadID(void) {
    return (pid_t) syscall(SYS_gettid);
}

int write_line(const char *msg) {
    pthread_mutex_lock(&g_lock);
    ssize_t n = write(g_fd, msg, strlen(msg));
    int res = 0;
    if (n < 0) {
        fprintf(stderr, "write() failed: %s, [file descr = %d]\n", strerror(errno), g_fd);
        res = -1;
    }
    pthread_mutex_unlock(&g_lock);
    return res;
}

void *func_thread(void *arg) {
    struct ThreadArgs *t = (struct ThreadArgs *)arg;
    char buf[128];
    struct timespec ts = {0, 100 * 1000 * 1000};

    for (int i = 0; i < COUNT_ITERATIONS; ++i) {
        snprintf(buf, sizeof(buf),
                 "[tag = %s] pid = %d ppid = %d tid = %d iter = %d\n",
                 t->tag, getpid(), getppid(), getThreadID(), i);
        write_line(buf);
        nanosleep(&ts, NULL);
    }
    return NULL;
}

void about(void) {
    printf("Pthread example\n");
}
