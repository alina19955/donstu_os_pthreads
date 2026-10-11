#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_ITEMS 20

// Общий буфер (одно место)
char g_buffer[256];
int  g_has_item = 0;         // 0 — буфер пуст, 1 — в буфере есть данные
int  g_done = 0;             // 0 — производитель работает, 1 — закончил

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  g_cond  = PTHREAD_COND_INITIALIZER;

// ---------- ПРОИЗВОДИТЕЛЬ ----------
void *producer(void *arg) {
    (void)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&g_mutex);

        // Ждём, пока потребитель заберёт предыдущий элемент
        while (g_has_item) {
            pthread_cond_wait(&g_cond, &g_mutex);
        }

        // Кладём новый элемент в буфер
        snprintf(g_buffer, sizeof(g_buffer),
                 "item %d from producer", i);
        g_has_item = 1;
        printf("producer: положил \"item %d\"\n", i);

        // Сообщаем потребителю
        pthread_cond_signal(&g_cond);
        pthread_mutex_unlock(&g_mutex);

        usleep(100000);   // 100 мс — имитация работы
    }

    // Сообщаем потребителю, что данных больше не будет
    pthread_mutex_lock(&g_mutex);
    g_done = 1;
    pthread_cond_signal(&g_cond);
    pthread_mutex_unlock(&g_mutex);

    return NULL;
}

// ---------- ПОТРЕБИТЕЛЬ ----------
void *consumer(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&g_mutex);

        // Ждём, пока появится элемент ИЛИ производитель закончит
        while (!g_has_item && !g_done) {
            pthread_cond_wait(&g_cond, &g_mutex);
        }

        // Если производитель закончил и буфер пуст — выходим
        if (!g_has_item && g_done) {
            pthread_mutex_unlock(&g_mutex);
            break;
        }

        // Забираем элемент из буфера
        printf("consumer: забрал \"%s\"", g_buffer);
        g_has_item = 0;

        // Сообщаем производителю, что буфер свободен
        pthread_cond_signal(&g_cond);
        pthread_mutex_unlock(&g_mutex);

        usleep(150000);   // 150 мс — потребитель медленнее
    }

    printf("consumer: производитель закончил, выходим\n");
    return NULL;
}

int main(void) {
    pthread_t prod, cons;

    printf("=== Задание 30: производитель-потребитель (1 буфер) ===\n");

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);

    printf("=== Готово ===\n");
    return 0;
}
