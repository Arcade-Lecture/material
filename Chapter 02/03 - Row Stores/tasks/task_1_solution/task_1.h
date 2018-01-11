#pragma once

#include <gs_attr.h>
#include <gs_frag.h>
#include <gs_tuplet_field.h>
#include "../../help/exercice_helper.h"
#include "../reference/reference_scan_impl.h"
#include "task_1_scan_impl.h"

// Optimization possibility 1: avoid function pointer calls in scan implementation,
//         for this, copy the evaluation function bodies from exercise_helper.h to which they are used in
//         taks_1scan_impl.h.
//         -> almost no speedup

// Optimization possibility 2: avoid multiple function calls to scan_impl and put everything into one function
//         -> speedup around 1.2


static inline size_t run_task_1_solution(size_t num_tuples)
{
    gs_frag_t *lineitem = help_create_lineitem_table(num_tuples);
    help_fill_lineitem_table(lineitem, num_tuples);


    printf("$ SELECT * FROM lineitem LIMIT 10;\n");
    gs_frag_print(stdout, lineitem, 0, 10);

    printf("\n$ SELECT * FROM lineitem WHERE shipdate >= 1017679567 AND quantity < 24 AND discount BETWEEN 0.5 AND 0.7 LIMIT 15;\n");
    uint64_t timer_start = TIMER_STOP();
    gs_frag_t *q1 = task_1_scan_impl_solution(lineitem, "quantity", exercice_pred_less, &(uint32_t) {24},
                                              "shipdate", exercice_pred_greatereq, &(uint64_t) {1017679567},
                                              "discount", exercice_pred_greatereq, &(float) {0.5f},
                                              "discount", exercice_pred_lesseq, &(float) {0.7f});
    uint64_t timer_stop = TIMER_STOP();
    gs_frag_print(stdout, q1, 0, 15);
    size_t time = timer_stop - timer_start;
    printf("%zu records in %zums\n\n", gs_frag_num_of_tuplets (q1), time);

    //gs_frag_delete(q1);
    gs_frag_delete(lineitem);
    return time;
}