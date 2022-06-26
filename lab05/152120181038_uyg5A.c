#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUMOFITEMS 13
pthread_mutex_t mt;

int count = 0;

sem_t semaphore_1, semaphore_2, semaphore_3, semaphore_4, semaphore_5, semaphore_6;//6 tane smepahore işimizi gördü.
void dressItem(int part, char* name) {

    static const char* BODY_STRING[] = { "kafa ", "govde", "el   ", "bacak", "ayak " };

    //count 0'sa ilk işlemdir.
    if (count == 0) {

        printf("%2d. \t\t\t\t\t--> Saglik calisanlarina sonsuz tesekkurler!..\n", count);
        count++;
    }
    printf("%2d. (%s) %s ", count, BODY_STRING[part], name);

    if (count == 13) { //13 olduğunda göreve hazır hale gelmiş oluyor.

        printf("\t--> Goreve hazirim!\n");
    }

    else if (count > 6) { //count 5'dan büyük olduğu an yarısından az malzeme kalmış demektir.

        printf("\t--> Kalan oge sayisi: %2d\n", 13 - count);
        count++;
    }

    else {

        printf("\n");
        count++;
    }


}

void funcHEAD(void* p) {
    sem_wait(&semaphore_4);
    pthread_mutex_lock(&mt);



    dressItem(0, "YuzDezenfektani          ");

    pthread_mutex_unlock(&mt);
    sem_wait(&semaphore_2);


    dressItem(0, "SaglikMaskesi            ");
    dressItem(0, "Bone                     ");

    sem_post(&semaphore_3);

    sem_wait(&semaphore_4);

    dressItem(0, "KoruyucuPlastikYuzMaskesi");

}

void funcUBDY(void* p) {
    dressItem(1, "Atlet                    ");
    dressItem(1, "Gomlek                   ");

    sem_wait(&semaphore_3);

    dressItem(1, "Tulum                    ");

    sem_post(&semaphore_4);
}

void funcHAND(void* p) {

    sem_wait(&semaphore_5);
    pthread_mutex_lock(&mt);

    dressItem(2, "ElDezenfektani           ");
    dressItem(2, "Eldiven                  ");

    pthread_mutex_unlock(&mt);

    sem_post(&semaphore_2);
}

void funcLEG_(void* p) {
    sem_wait(&semaphore_1);

    dressItem(3, "Pantolon                 ");
    dressItem(3, "Kemer                    ");

    sem_post(&semaphore_6);
}

void funcFOOT(void* p) {
    dressItem(4, "Corap                    ");

    sem_post(&semaphore_1);
    sem_wait(&semaphore_6);

    dressItem(4, "Ayakkabi                 ");

    sem_post(&semaphore_4);
    sem_post(&semaphore_5);
}

int main(int argc, char* argv[]) {

    pthread_t bas, govde, el, bacak, ayak;

    sem_init(&semaphore_1, 0, 0);
    sem_init(&semaphore_2, 0, 0);
    sem_init(&semaphore_3, 0, 0);
    sem_init(&semaphore_4, 0, 0);
    sem_init(&semaphore_5, 0, 0);
    sem_init(&semaphore_6, 0, 0);

    pthread_create(&govde, NULL, funcHEAD, NULL);
    pthread_create(&ayak, NULL, funcUBDY, NULL);
    pthread_create(&bacak, NULL, funcHAND, NULL);
    pthread_create(&bas, NULL, funcLEG_, NULL);
    pthread_create(&el, NULL, funcFOOT, NULL);

    pthread_join(govde, NULL);
    pthread_join(ayak, NULL);
    pthread_join(bacak, NULL);
    pthread_join(bas, NULL);
    pthread_join(el, NULL);
    return 0;
}