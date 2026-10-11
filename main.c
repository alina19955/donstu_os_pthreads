#include "pthreadfuncs.h"

int main(void) {
    about();
    
    pthread_t threads[COUNT_THREADS];
    struct ThreadArgs args[COUNT_THREADS];

    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "main: pid = %d, opened file: 'output.log' (fd = %d)\n", getThreadID(), g_fd);
    write_line(log_buf);

    // Задание 7: Формирование аргументов в цикле
    for (int i = 0; i < COUNT_THREADS; i++) {
        args[i].id = i + 1;
        snprintf(args[i].tag, sizeof(args[i].tag), "T%d", i + 1);
    }

    // Создание потоков
    for (int i = 0; i < COUNT_THREADS; i++) {
        int rc = pthread_create(&threads[i], NULL, func_thread, &args[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            return EXIT_FAILURE;
        }
    }

    // Ожидание завершения потоков
    for (int i = 0; i < COUNT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    if (close(g_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }

    pthread_mutex_destroy(&g_lock);
    printf("main: all threads finished, file closed\n");
    return EXIT_SUCCESS;
}
