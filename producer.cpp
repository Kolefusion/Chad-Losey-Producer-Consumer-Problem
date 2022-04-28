// Chad Losey
// producer.cpp
// This program places ints in shared buffer and outputs what it put in

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
    // make a shared memory object
    int fd = shm_open("/shm.tmp", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, sizeof(shmbuf));
    struct shmbuf* shmp = static_cast<shmbuf*>(mmap(NULL, sizeof(*shmp),
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    // initialize semaphores
    sem_init(&shmp->remainingSpots, 1, 2);
    sem_init(&shmp->itemsReady, 1, 0);

    srand(time(NULL));
    int i = 0;

    while (true) {
        // semaphore initially 2, waits when buffer is full
        sem_wait(&shmp->remainingSpots);

        // produces random int to buffer and outputs what it produced
        shmp->buf[i] = rand() % 99 + 1;
        std::cout << "produced item: " << shmp->buf[i] << std::endl;
        i = (i + 1) % 2;

        // allows consumer to consume another item
        sem_post(&shmp->itemsReady);

        // sleeps for random time to allow for variation in buffer fullness
        sleep(rand() % 4 + 1);
    }
}