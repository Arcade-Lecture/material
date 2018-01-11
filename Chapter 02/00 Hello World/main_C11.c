

#include <stddef.h>

#define DATA 10000000
#define MAX 10000000

// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
#define random_at_most(max)                             \
({                                                      \
    unsigned long                                       \
            num_bins = (unsigned long) max + 1,         \
            num_rand = (unsigned long) RAND_MAX + 1,    \
            bin_size = num_rand / num_bins,             \
            defect   = num_rand % num_bins;             \
                                                        \
    long x;                                             \
    do {                                                \
        x = random();                                   \
    }                                                   \
    while (num_rand - defect <= (unsigned long)x);      \
    (x/bin_size);                                       \
})


/*char data[DATA];


int main(void)
{
    struct timeval t0, t1;
    long elapsed = 0;
    long sum = 0;

    for (int i = 0; i < MAX; i++) {
        gettimeofday(&t0, 0);
        sum += *(data + random_at_most(DATA - 1)) % 2 == 0;
        gettimeofday(&t1, 0);
        elapsed += (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
    }
    printf("%f\n", elapsed / (float) MAX);

    // 0.140069

    return 0;
}*/


int main(void)
{
    int *p = NULL;
    printf("%zu\n\n", sizeof(void*));
    char *data = malloc(DATA);
    struct timeval t0, t1;
    double elapsed = 0;
    long sum = 0;

    for (int i = 0; i < MAX; i++) {
        gettimeofday(&t0, 0);
        sum += *(data + random_at_most(DATA - 1)) % 2 == 0;
        gettimeofday(&t1, 0);
        elapsed += (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
    }
    printf("%f\t%l\n", elapsed / (double) MAX, sum);
    free(data);
    //0.221090
}