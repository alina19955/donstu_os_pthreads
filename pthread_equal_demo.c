#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t t1_id;
pthread_t t2_id;

void *thread1(void *arg) {
    (void)arg;
    pthread_t self_in_thread = pthread_self();

    printf("T1: pthread_self() в потоке       = %lu\n",
           (unsigned long)self_in_thread);
    printf("T1: сохранённый pthread_t из main = %lu\n",
           (unsigned long)t1_id);

    int eq = pthread_equal(self_in_thread, t1_id);
    printf("T1: pthread_equal(self, t1_id) = %d  (1 = равны)\n", eq);

    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    printf("T2: pthread_self() = %lu\n", (unsigned long)pthread_self());
    return NULL;
}

int main(void) {
    pthread_create(&t1_id, NULL, thread1, NULL);
    pthread_create(&t2_id, NULL, thread2, NULL);

    pthread_join(t1_id, NULL);
    pthread_join(t2_id, NULL);

    printf("\n--- Сравнение двух разных потоков ---\n");
    printf("t1_id = %lu\n", (unsigned long)t1_id);
    printf("t2_id = %lu\n", (unsigned long)t2_id);

    int eq = pthread_equal(t1_id, t2_id);
    printf("pthread_equal(t1_id, t2_id) = %d  (0 = разные)\n", eq);

    // Проверка: pthread_equal(t, t) всегда 1
    int eq_self = pthread_equal(t1_id, t1_id);
    printf("pthread_equal(t1_id, t1_id) = %d  (1 = равны)\n", eq_self);

    return 0;
}
