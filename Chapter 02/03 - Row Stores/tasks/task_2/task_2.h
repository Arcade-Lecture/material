#pragma once

#include <gs_frag.h>
#include "../../help/exercice_helper.h"
#include "../reference/reference_scan_impl.h"

static inline void run_task_2(size_t num_tuples)
{
    gs_frag_t *lineitem = help_create_lineitem_table(num_tuples);
    help_fill_lineitem_table(lineitem, num_tuples);

    printf("\n$ SELECT * FROM lineitem WHERE comment == '!!!';\n");
    uint64_t timer_start = TIMER_STOP();
    gs_frag_t *q1 = reference_scan_impl(lineitem, "comment", exercice_pred_equals, "!!!");
    uint64_t timer_stop = TIMER_STOP();
    gs_frag_print(stdout, q1, 0, INT_MAX);
    printf("%llums\n\n", (timer_stop - timer_start));

    gs_frag_delete(q1);
    gs_frag_delete(lineitem);
}
