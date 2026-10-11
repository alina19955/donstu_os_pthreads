#include "pthreadfuncs.h"

int g_fd = -1;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

pid_t getThreadID(void) {
    return (pid_t)syscall(SYS_gettid);
}

int write_line(const char *msg) {
//    pthread_mutex_lock(&g_lock);

    ssize_t n = write(g_fd, msg, strlen(msg));
    int res = 0;

    if (n < 0) {
        fprintf(stderr,
                "write() failed: %s, [file descr = %d]\n",
                strerror(errno), g_fd);
        res = -1;
    }

//    pthread_mutex_unlock(&g_lock);

    return res;
}

void *func_thread(void *arg) {
    struct ThreadArgs *t = (struct ThreadArgs *)arg;
    char buf[256];

    // Задание 26: поток detached, структура в куче
    pthread_detach(pthread_self());

    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 1000000;

    // Задание 24: печатаем message из структуры
    snprintf(buf, sizeof(buf),
             "[tag = %s] message = \"%s\"\n",
             t->tag, t->message);
    write_line(buf);

    unsigned long pthread_id = (unsigned long)pthread_self();

    for (int i = 0; i < COUNT_ITERATIONS; ++i) {

        // Общий счётчик защищён mutex
        pthread_mutex_lock(&g_lock);
        counter++;
        pthread_mutex_unlock(&g_lock);

        snprintf(buf, sizeof(buf),
                 "[tag = %s] pid = %d tid = %d pthread_self = %lu iter = %d\n",
                 t->tag,
                 getpid(),
                 getThreadID(),
                 pthread_id,
                 i);

        if (write_line(buf) != 0) {
            fprintf(stderr, "T%d: write_line failed\n", t->id);
        }

        nanosleep(&ts, NULL);   // задание 5: задержка 1 мс
    }

    // Задание 26: освобождаем структуру, переданную из main через malloc
    free(t);

    // Задание 25: возврат malloc-строки с числом итераций
    char *result = malloc(32);
    if (result == NULL) {
        pthread_exit(NULL);
    }
    snprintf(result, 32, "%d", COUNT_ITERATIONS);
    pthread_exit((void *)result);
}

void about(void) {
    printf("Pthread example\n");
}
