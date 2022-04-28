// Chad Losey
// consumer.cpp
// This program reads ints from shared buffer and outputs what it read

#include <semaphore.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

// structure for shared memory
struct shmbuf {
    sem_t  remainingSpots;
    sem_t  itemsReady;
    int   buf[2];
};

int main() {
    // get access to shared memory object
    sleep(2);
    int fd = shm_open("/shm.tmp", O_RDWR, 0);
    struct shmbuf *shmp = static_cast<shmbuf*>(mmap(NULL, sizeof(*shmp),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    srand(time(NULL));
    int i = 0;

    while(true) {
        // semaphore initially 0, waits when buffer is empty
        sem_wait(&shmp->itemsReady);

        // consumes item from buffer and outputs what it consumed
        std::cout << "  consumed item: " << shmp->buf[i] << std::endl;
        i = (i + 1) % 2;

        // allows producer to produce another item
        sem_post(&shmp->remainingSpots);

        // sleeps for random time to allow for variation in buffer fullness
        sleep(rand()%4 + 1);
    }
}