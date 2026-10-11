#include "pthreadfuncs.h"

int main(void) {
    about();

    pthread_t threads[COUNT_THREADS];
    struct ThreadArgs *args[COUNT_THREADS];   // Задание 26: указатели, не массив

    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    char log_buf[128];

    snprintf(log_buf, sizeof(log_buf),
             "main: pid = %d, opened file: 'output.log' (fd = %d)\n",
             getThreadID(), g_fd);

    if (write_line(log_buf) != 0) {
        fprintf(stderr, "main: write_line failed\n");
        close(g_fd);
        return EXIT_FAILURE;
    }

    // Задание 26: каждая структура — в куче через malloc
    for (int i = 0; i < COUNT_THREADS; i++) {
        args[i] = malloc(sizeof(struct ThreadArgs));
        if (args[i] == NULL) {
            perror("malloc");
            close(g_fd);
            return EXIT_FAILURE;
        }

        args[i]->id = i + 1;

        snprintf(args[i]->tag,
                 sizeof(args[i]->tag),
                 "T%d",
                 i + 1);

        snprintf(args[i]->message,
                 sizeof(args[i]->message),
                 "Hello from thread T%d",
                 i + 1);
    }

    // Создание потоков
    for (int i = 0; i < COUNT_THREADS; i++) {
        int rc = pthread_create(&threads[i],
                                NULL,
                                func_thread,
                                args[i]);

        if (rc != 0) {
            fprintf(stderr,
                    "pthread_create: %s\n",
                    strerror(rc));

            close(g_fd);
            return EXIT_FAILURE;
        }
    }

    // Задание 26: потоки detached, pthread_join невозможен.
    // Ждём их завершения через sleep.
    printf("main: waiting for detached threads...\n");
    sleep(15);   // 10000 итераций × 1 мс ≈ 10 секунд + запас

    printf("main: counter = %d\n", counter);

    if (close(g_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }

    pthread_mutex_destroy(&g_lock);

    printf("main: all threads finished, file closed\n");

    return EXIT_SUCCESS;
}
