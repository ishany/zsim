#include <iostream>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <condition_variable>

#include <pthread.h>

inline int MatIndex(int mat_size, int row, int col) {
    return row * mat_size + col;
}

typedef struct _global_data {
    int mat_size;
    int num_cores;
    int * mat_a;
    int * mat_b;
    int * mat_o;
} global_data;

static inline void compute_element(
    global_data * gd,
    int row,
    int col)
{
    int * mat_a = gd->mat_a;
    int * mat_b = gd->mat_b;
    int * mat_o = gd->mat_o;

    for (int i = 0; i < gd->mat_size; i++) {

        mat_o[MatIndex(gd->mat_size, row, col)] +=
            mat_a[MatIndex(gd->mat_size, row, i)] *
            mat_b[MatIndex(gd->mat_size, i, col)];
    }
}


void worker(int id, global_data * gd) {
    int row;
    int col;

    for (id; id < gd->mat_size * gd->mat_size; id += gd->num_cores) {
        row = id / gd->mat_size;
        col = id % gd->mat_size;

        compute_element(gd, row, col);
    }
}

void worker_entry(int id, global_data * gd) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(id, &cpuset);
    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    worker(id, gd);
}

void gen_csv(int * dest, const size_t mat_size) {
    for (size_t i = 0; i < mat_size * mat_size; i++) {
        dest[i] = rand() % 100;
    }
}

int main(int argc, char * argv[]) {
    int num_cores = atoi(argv[1]);
    int mat_size = atoi(argv[2]);

    global_data gd;
    gd.num_cores = num_cores;
    gd.mat_size = mat_size;
    gd.mat_a = new int[mat_size * mat_size];
    gd.mat_b = new int[mat_size * mat_size];
    gd.mat_o = new int[mat_size * mat_size];

    std::vector<std::thread> threads;

    printf("Reading matrices\n");
    gen_csv(gd.mat_a, mat_size);
    gen_csv(gd.mat_b, mat_size);

    memset(gd.mat_o, 0x00, mat_size * mat_size * sizeof(int));

    printf("Setting affinity for thread 0\n");
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    printf("Spawning worker threads\n");
    for (int id = 1; id < num_cores; id++) {
        threads.push_back(std::thread(worker, id, &gd));
    }

    printf("Working...\n");
    worker(0, &gd);

    printf("Waiting for threads...\n");
    for (int id = 0; id < num_cores - 1; id++) {
        threads[id].join();
    }

    for (int row = 0; row < mat_size; row++) {
        for (int col = 0; col < mat_size; col++) {
            printf("%d, ", gd.mat_o[MatIndex(mat_size, row, col)]);
        }
        printf("\n");
    }

    delete gd.mat_a;
    delete gd.mat_b;
    delete gd.mat_o;

    return 0;
}
