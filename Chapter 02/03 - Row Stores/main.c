#include <stdio.h>

#include "help/exercice_helper.h"
#include "tasks/reference/reference_implementation.h"
#include "tasks/task_1/task_1.h"
#include "tasks/task_2/task_2.h"
#include "tasks/task_1_solution/task_1.h"

#define NUM_TUPLES 1000000

int main() {
    apr_initialize();

    size_t original_time = reference_implementation(NUM_TUPLES);
    size_t your_time = run_task_1_solution(NUM_TUPLES); // run_task_1(NUM_TUPLES);


    fflush(stdout);
    fprintf(stderr, "Your speedup: %f", original_time / (float) your_time);

    run_task_2(NUM_TUPLES);

    return 0;
}




