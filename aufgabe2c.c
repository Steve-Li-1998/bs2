#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

int terminCounter = 50;
pthread_mutex_t lock;

void* newHotline(void *arg){
    while (1){
        pthread_mutex_lock(&lock);
        if (terminCounter <= 0){
            pthread_mutex_unlock(&lock);
            break;
        }
        printf("Es gibt aktuell noch %d verfügbare Terminen\n",terminCounter);
        sleep(1);
        terminCounter--;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main() {
    int status[6];
    pthread_t pthread[6];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < 6; ++i) {
        status[i] = pthread_create(&pthread[i], NULL, &newHotline, NULL);
        if (status[i]){
            perror("Das Thread kann nicht erzeugt werden! Vorgang wird abgebrochen.");
            for (int j = 0; j < i; ++j) {
                pthread_kill(pthread[j], SIGKILL);
            }
        }
    }

    for (int i = 0; i < 6; ++i) {
        status[i] = pthread_join(pthread[i], NULL);
        if (status[i]){
            perror("pthread_join fail");
        }
    }
    pthread_mutex_destroy(&lock);
    printf("Die Anzahl der verbleibenden Terminen ist: %d\n", terminCounter);
    pthread_exit(NULL);
}
