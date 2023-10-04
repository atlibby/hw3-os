#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMVALS (1024*1024)
#define NUMTHREADS 1

float g_vals[NUMVALS];

typedef struct {
    int startIndex; // start index at which to start looking
    int endIndex; // end index at which to stop looking
    float threshold; // threshold value
    int count; // result: #values that are greater than threshold
} CountInfo;

void *doCount(void *);
int prand();
void *runner(void *);


int main() {
    CountInfo tdata[NUMTHREADS];    // holds data we want to give to each thread
    pthread_t tids[NUMTHREADS];     // thread identifier for child thread #1

    // initialize the array with random integers
    int i1, i2;
    float f1, f2;
    for (int i=0; i<NUMVALS; ++i) {
        i1 = prand(100);
        i2 = prand(100);
        f1 = i1 / 100.0;
        f2 = i2 / 100.0;
        g_vals[i] = f1 / (1.0 + f2);
    }

    int i, pos, chunkSize;

    // set up bounds for the threads
    chunkSize = NUMVALS / NUMTHREADS;
    pos = 0;
    for (i=0; i<NUMTHREADS; ++i) {
        tdata[i].startIndex = pos;
        tdata[i].endIndex = tdata[i].startIndex + chunkSize;
        tdata[i].threshold = 0.5;
        pos = pos + chunkSize + 1;
    }

    // adjust last region if necessary
    if (tdata[NUMTHREADS-1].endIndex < NUMVALS-1)
        tdata[NUMTHREADS-1].endIndex = NUMVALS - 1;
    else if (tdata[NUMTHREADS-1].endIndex > NUMVALS-1)
        tdata[NUMTHREADS-1].endIndex = NUMVALS - 1;

    // create child threads
    for (i=0; i<NUMTHREADS; ++i)
        pthread_create(&tids[i], NULL, doCount, &tdata[i]);

    // wait for the child threads to terminate
    for (i=0; i<NUMTHREADS; ++i)
        pthread_join(tids[i], NULL);

    printf("count %d\n", tdata[0].count);
    return 0;
}

void *doCount(void *param) {
    CountInfo *data = (CountInfo *)param;
    int count = 0;

    for (int i = data->startIndex; i <= data->endIndex; ++i) {
        if (g_vals[i] > data->threshold) {
            count++;
        }
    }
    data->count = count;
    pthread_exit(NULL);
}
int prand() {
    static int p = 1;
    const int a = 105491;
    const int b = 733;
    const int c = 6619;
    const int base = 100;
    int val;
    p = p*a + b;
    p = p % c;
    val = p % base;
    return val;
}



