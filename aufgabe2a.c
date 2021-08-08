#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

int terminCounter = 50;

void* newHotline(void *arg){
    while (terminCounter > 0){
        printf("Es gibt aktuell noch %d verfügbare Terminen\n",terminCounter);
        sleep(1);
        terminCounter--;
    }
    pthread_exit(NULL);
}

int main() {
    int status[6];
    pthread_t pthread[6];

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
    printf("Die Anzahl der verbleibenden Terminen ist: %d\n", terminCounter);
    pthread_exit(NULL);
}
