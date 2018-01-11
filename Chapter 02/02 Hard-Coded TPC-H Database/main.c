#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define NUM_TUPLES 50000
#define MAX_LINE_LENGTH 1024

static bool readline(char buffer[]);
static int index_of(char str[], char needle[]);

struct nsm_lineitem_tuple_t {
    uint32_t orderkey;
    uint32_t partkey;
    uint32_t suppkey;
    int32_t linenumber;
    float quantity;
    float extendedprice;
    float discount;
    float tax;
    char returnflag;
    char linestatus;
    uint64_t shipdate;
    uint64_t commitdate;
    uint64_t receipetdate;
    char shipinstr[25];
    char shipmode[10];
    char comment[44];
};

struct nsm_lineitem_table_t {
    struct nsm_lineitem_tuple_t tuples[NUM_TUPLES];
};

int main(void)
{
    struct nsm_lineitem_table_t nsm_table;

    /* fill with random data */
    for (int i = 0; i < NUM_TUPLES; i++) {
        nsm_table.tuples[i].orderkey      = random() % UINT32_MAX;
        nsm_table.tuples[i].partkey       = random() % UINT32_MAX;
        nsm_table.tuples[i].suppkey       = random() % UINT32_MAX;
        nsm_table.tuples[i].linenumber    = random() % INT32_MAX;
        nsm_table.tuples[i].quantity      = random() % 1000;
        nsm_table.tuples[i].extendedprice = random() % 10000;
        nsm_table.tuples[i].discount      = random() % 50 / 100.0f;
        nsm_table.tuples[i].tax           = nsm_table.tuples[i].extendedprice * 0.17f;
        nsm_table.tuples[i].returnflag    = 'a' + (random() % 26);
        nsm_table.tuples[i].linestatus    = 'a' + (random() % 26);
        nsm_table.tuples[i].shipdate      = random() % UINT64_MAX;
        nsm_table.tuples[i].commitdate    = random() % UINT64_MAX;
        nsm_table.tuples[i].receipetdate  = random() % UINT64_MAX;
        for (int j = 0; j < 25; j++) {
            nsm_table.tuples[i].shipinstr[j] = 'a' + (random() % 26);
        }
        for (int j = 0; j < 10; j++) {
            nsm_table.tuples[i].shipmode[j]  = 'a' + (random() % 26);
        }
        for (int j = 0; j < 44; j++) {
            nsm_table.tuples[i].comment[j]   = 'a' + (random() % 26);
        }
    }

    char needle[256], limitstr[256];
    printf("Type ':exit' to exit.\n");
    printf("Database size: %f MiB\n", sizeof(struct nsm_lineitem_table_t) / 1024.0f / 1024.0f);

    for(;;) {
        printf("SELECT orderkey, partkey, suppkey, comment FROM lineitem WHERE comment LIKE %%x%% LIMIT n.\n");

        printf("Enter x");
        if (!readline(needle))
            break;
        printf("Enter n (-1 for no limit)");
        if (!readline(limitstr))
            break;

        int limit = atoi(limitstr);
        unsigned result_size = 0;
        struct timeval timeval;
        long start, end;

        /* print tuples to std out */
        gettimeofday(&timeval, NULL);
        start = (long)timeval.tv_sec * 1000 + (long)timeval.tv_usec / 1000;
        for (unsigned i = 0, limit_counter = 0; i < NUM_TUPLES; i++) {
            if (index_of(nsm_table.tuples[i].comment, needle) >= 0) {
                result_size++;
                if (limit_counter++ < limit) {
                    printf("%ud, %ud, %ud, %s\n",
                           nsm_table.tuples[i].orderkey,
                           nsm_table.tuples[i].partkey,
                           nsm_table.tuples[i].suppkey,
                           nsm_table.tuples[i].comment);
                }
            }
        }
        gettimeofday(&timeval, NULL);
        end = (long)timeval.tv_sec * 1000 + (long)timeval.tv_usec / 1000;

        printf("%d records in total, %ld ms\n", result_size, (end - start));
    }

    return EXIT_SUCCESS;
}

/* taken from lecture slides */
static int index_of(char str[], char needle[])
{
    register char str_c, str_scan_c, needle_c;
    register int match_idx;
    register bool match_found;

    for (match_idx = 0, match_found = false;
         str_c = str[match_idx], str_c != '\0' && !match_found;
         match_idx++) {
        for (unsigned j = 0; needle_c = needle[j], str_scan_c = str[match_idx + j],
                str_c == needle[0] && needle_c != '\0' && str_scan_c != '\0' && str_scan_c == needle_c;
             j++) {
            match_found |= (needle[j + 1] == '\0');
        }
    }
    return match_found ? match_idx : -1;
}

/* taken from lecture slides */
static bool readline(char buffer[])
{
    printf("$ ");
    fgets(buffer, MAX_LINE_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    return (strcmp(buffer, ":exit\n") != 0);
}