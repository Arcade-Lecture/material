#pragma once

#include <gs_attr.h>
#include <gs_frag.h>
#include <gs_tuplet_field.h>
#include "../../help/exercice_helper.h"

#define NUM_THREADS 16

typedef struct worker_args_t {
    gs_frag_t *table;
    size_t *positions;
    size_t num_positions;
    size_t tid;
    size_t begin;
    size_t chunksize;
    const char *attr_name_1; exercice_pred_e pred_1; const void *data_1;
    const char *attr_name_2; exercice_pred_e pred_2; const void *data_2;
    const char *attr_name_3; exercice_pred_e pred_3; const void *data_3;
    const char *attr_name_4; exercice_pred_e pred_4; const void *data_4;
} worker_args_t;

static inline void *thread_task(void *data);

static inline gs_frag_t *task_1_scan_impl_solution(gs_frag_t *table,
                                                   const char *attr_name_1, exercice_pred_e pred_1, const void *data_1,
                                                   const char *attr_name_2, exercice_pred_e pred_2, const void *data_2,
                                                   const char *attr_name_3, exercice_pred_e pred_3, const void *data_3,
                                                   const char *attr_name_4, exercice_pred_e pred_4, const void *data_4)
{
    pthread_t worker[NUM_THREADS];
    worker_args_t worker_args[NUM_THREADS];

    size_t chunk_size = gs_frag_num_of_tuplets(table) / NUM_THREADS;
    printf(">> %zu --- chunksize: %zu \n", gs_frag_num_of_tuplets(table), chunk_size);

    for (int i = 0; i < NUM_THREADS; i++) {
        worker_args[i].tid = i;
        worker_args[i].begin = i * chunk_size;
        worker_args[i].chunksize = chunk_size;
        worker_args[i].table = table;
        worker_args[i].attr_name_1 = attr_name_1;
        worker_args[i].pred_1 = pred_1;
        worker_args[i].data_1 = data_1;
        worker_args[i].attr_name_2 = attr_name_2;
        worker_args[i].pred_2 = pred_2;
        worker_args[i].data_2 = data_2;
        worker_args[i].attr_name_3 = attr_name_3;
        worker_args[i].pred_3 = pred_3;
        worker_args[i].data_3 = data_3;
        worker_args[i].attr_name_4 = attr_name_4;
        worker_args[i].pred_4 = pred_4;
        worker_args[i].data_4 = data_4;
        pthread_create(&worker[i], NULL, thread_task, &worker_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(worker[i], NULL);
    }

    // Copy results together

    gs_tuplet_t input_tuplet, output_tuplet;
    gs_tuplet_field_t input_field, output_field;
    gs_frag_t *result = gs_frag_new(table->schema, table->ntuplets, FIT_HOST_NSM_VM);

    for (int i = 0; i < NUM_THREADS; i++) {
        gs_tuplet_open(&input_tuplet, worker_args[i].table, 0);
        do {
            gs_tuplet_field_seek(&input_field, &input_tuplet, 0);
            gs_frag_insert(&output_tuplet, result, 1);
            gs_tuplet_field_open(&output_field, &output_tuplet);
            for (int i = 0; i < gs_schema_num_attributes(table->schema); i++) {
                const void *in_data = gs_tuplet_field_read(&input_field);
                gs_tuplet_field_write(&output_field,
                                      gs_schema_attr_type(table->schema, i) != FT_CHAR ? in_data : &in_data, true);
                gs_tuplet_field_next(&input_field, false);
            }

        } while (gs_tuplet_next(&input_tuplet));
    }


    return result;
}

static inline void *thread_task(void *data) {

    worker_args_t *args = (worker_args_t *) data;

    gs_tuplet_t input_tuplet, output_tuplet;
    gs_tuplet_field_t input_field_1, input_field_2, input_field_3, input_field_4, output_field;
    gs_tuplet_open(&input_tuplet, args->table, 0);

    const gs_attr_t *attribute_1 = gs_schema_attr_by_name(args->table->schema, args->attr_name_1);
    const gs_attr_t *attribute_2 = gs_schema_attr_by_name(args->table->schema, args->attr_name_2);
    const gs_attr_t *attribute_3 = gs_schema_attr_by_name(args->table->schema, args->attr_name_3);
    const gs_attr_t *attribute_4 = gs_schema_attr_by_name(args->table->schema, args->attr_name_4);

    gs_frag_t *result = gs_frag_new(args->table->schema, args->table->ntuplets, FIT_HOST_NSM_VM);

    size_t current_pos = 0;
    size_t current_chunk_i = 0;

    args->chunksize = args->tid == NUM_THREADS - 1 ? args->chunksize - 1 : args->chunksize;
    do {
        if (current_pos < args->begin) {
            current_pos++;
            continue;
        }

        if (current_chunk_i++ >= args->chunksize) {
            printf("\nSTOP TID %zu [%zu, %zu)\n", args->tid, current_pos, current_pos + args->chunksize);
            break;
        }



        gs_tuplet_field_open(&input_field_1, &input_tuplet);
        gs_tuplet_field_open(&input_field_2, &input_tuplet);
        gs_tuplet_field_open(&input_field_3, &input_tuplet);
        gs_tuplet_field_open(&input_field_4, &input_tuplet);

        gs_tuplet_field_seek(&input_field_1, &input_tuplet, attribute_1->id);
        gs_tuplet_field_seek(&input_field_2, &input_tuplet, attribute_2->id);
        gs_tuplet_field_seek(&input_field_3, &input_tuplet, attribute_3->id);
        gs_tuplet_field_seek(&input_field_4, &input_tuplet, attribute_4->id);

        const void *field_data_1 = gs_tuplet_field_read(&input_field_1);
        const void *field_data_2 = gs_tuplet_field_read(&input_field_2);
        const void *field_data_3 = gs_tuplet_field_read(&input_field_3);
        const void *field_data_4 = gs_tuplet_field_read(&input_field_4);

        int compare_1, compare_2, compare_3, compare_4;
        switch (attribute_1->type) {
            case FT_INT32:
                compare_1 = *(int32_t *)field_data_1 - *(int32_t *)args->data_1;
                break;
            case FT_UINT32:
                compare_1 = (int64_t)*(uint32_t *)field_data_1 - (int64_t)*(uint32_t *)args->data_1;
                break;
            case FT_UINT64:
                compare_1 = ((int64_t)*(uint64_t *)field_data_1) - ((int64_t)*(uint64_t *)args->data_1);
                break;
            case FT_FLOAT32:
                compare_1 = (1000 * *(float *)field_data_1) - (1000 * *(float *)args->data_1);
                break;
            case FT_CHAR:
                compare_1 = strcmp((char *)field_data_1, (char *)args->data_1);
                break;
            default: panic("not implemented '%d'", attribute_1->type);
        }

        switch (attribute_2->type) {
            case FT_INT32:
                compare_2 = *(int32_t *)field_data_2 - *(int32_t *)args->data_2;
                break;
            case FT_UINT32:
                compare_2 = (int64_t)*(uint32_t *)field_data_2 - (int64_t)*(uint32_t *)args->data_2;
                break;
            case FT_UINT64:
                compare_2 = ((int64_t)*(uint64_t *)field_data_2) - ((int64_t)*(uint64_t *)args->data_2);
                break;
            case FT_FLOAT32:
                compare_2 = (1000 * *(float *)field_data_2) - (1000 * *(float *)args->data_2);
                break;
            case FT_CHAR:
                compare_2 = strcmp((char *)field_data_2, (char *)args->data_2);
                break;
            default: panic("not implemented '%d'", attribute_2->type);
        }

        switch (attribute_3->type) {
            case FT_INT32:
                compare_3 = *(int32_t *)field_data_3 - *(int32_t *)args->data_3;
                break;
            case FT_UINT32:
                compare_3 = (int64_t)*(uint32_t *)field_data_3 - (int64_t)*(uint32_t *)args->data_3;
                break;
            case FT_UINT64:
                compare_3 = ((int64_t)*(uint64_t *)field_data_3) - ((int64_t)*(uint64_t *)args->data_3);
                break;
            case FT_FLOAT32:
                compare_3 = (1000 * *(float *)field_data_3) - (1000 * *(float *)args->data_3);
                break;
            case FT_CHAR:
                compare_3 = strcmp((char *)field_data_3, (char *)args->data_3);
                break;
            default: panic("not implemented '%d'", attribute_3->type);
        }

        switch (attribute_4->type) {
            case FT_INT32:
                compare_4 = *(int32_t *)field_data_4 - *(int32_t *)args->data_4;
                break;
            case FT_UINT32:
                compare_4 = (int64_t)*(uint32_t *)field_data_4 - (int64_t)*(uint32_t *)args->data_4;
                break;
            case FT_UINT64:
                compare_4 = ((int64_t)*(uint64_t *)field_data_4) - ((int64_t)*(uint64_t *)args->data_4);
                break;
            case FT_FLOAT32:
                compare_4 = (1000 * *(float *)field_data_4) - (1000 * *(float *)args->data_4);
                break;
            case FT_CHAR:
                compare_4 = strcmp((char *)field_data_4, (char *)args->data_4);
                break;
            default: panic("not implemented '%d'", attribute_4->type);
        }

        bool match_1 = ((args->pred_1 == exercice_pred_less && compare_1 < 0) ||
                        (args->pred_1 == exercice_pred_greater && compare_1 > 0) ||
                        (args->pred_1 == exercice_pred_equals && compare_1 == 0) ||
                        (args->pred_1 == exercice_pred_lesseq && compare_1 <= 0) ||
                        (args->pred_1 == exercice_pred_greatereq && compare_1 >= 0) ||
                        (args->pred_1 == exercice_pred_unequal && compare_1 != 0));

        bool match_2 = ((args->pred_2 == exercice_pred_less && compare_2 < 0) ||
                        (args->pred_2 == exercice_pred_greater && compare_2 > 0) ||
                        (args->pred_2 == exercice_pred_equals && compare_2 == 0) ||
                        (args->pred_2 == exercice_pred_lesseq && compare_2 <= 0) ||
                        (args->pred_2 == exercice_pred_greatereq && compare_2 >= 0) ||
                        (args->pred_2 == exercice_pred_unequal && compare_2 != 0));

        bool match_3 = ((args->pred_3 == exercice_pred_less && compare_3 < 0) ||
                        (args->pred_3 == exercice_pred_greater && compare_3 > 0) ||
                        (args->pred_3 == exercice_pred_equals && compare_3 == 0) ||
                        (args->pred_3 == exercice_pred_lesseq && compare_3 <= 0) ||
                        (args->pred_3 == exercice_pred_greatereq && compare_3 >= 0) ||
                        (args->pred_3 == exercice_pred_unequal && compare_3 != 0));

        bool match_4 = ((args->pred_4 == exercice_pred_less && compare_4 < 0) ||
                        (args->pred_4 == exercice_pred_greater && compare_4 > 0) ||
                        (args->pred_4 == exercice_pred_equals && compare_4 == 0) ||
                        (args->pred_4 == exercice_pred_lesseq && compare_4 <= 0) ||
                        (args->pred_4 == exercice_pred_greatereq && compare_4 >= 0) ||
                        (args->pred_4 == exercice_pred_unequal && compare_4 != 0));
        if (match_1 && match_2 && match_3 && match_4) {
            gs_tuplet_field_seek(&input_field_1, &input_tuplet, 0);
            gs_frag_insert(&output_tuplet, result, 1);
            gs_tuplet_field_open(&output_field, &output_tuplet);

            for (int i = 0; i < gs_schema_num_attributes(args->table->schema); i++) {
                const void *in_data = gs_tuplet_field_read(&input_field_1);
                gs_tuplet_field_write(&output_field,
                                      gs_schema_attr_type(args->table->schema, i) != FT_CHAR ? in_data : &in_data, true);
                gs_tuplet_field_next(&input_field_1, false);
            }
        }

    } while (gs_tuplet_next(&input_tuplet));
    return NULL;
}