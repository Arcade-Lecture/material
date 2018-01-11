
#include <gs_frag.h>
#include <gs_tuplet_field.h>

#include "exercice_helper.h"

gs_frag_t *help_create_lineitem_table(size_t capacity) {

    gs_schema_t *lineitem_schema = gs_schema_new("lineitem");

    attr_create_uint32("orderkey", lineitem_schema);
    attr_create_uint32("partkey", lineitem_schema);
    attr_create_uint32("suppkey", lineitem_schema);
    attr_create_int32("linenumber", lineitem_schema);
    attr_create_uint32("quantity", lineitem_schema);
    attr_create_float32("extendedprice", lineitem_schema);
    attr_create_float32("discount", lineitem_schema);
    attr_create_float32("tax", lineitem_schema);
    attr_create_string("returnflag", 2, lineitem_schema);
    attr_create_string("linestatus", 2, lineitem_schema);
    attr_create_uint64("shipdate", lineitem_schema);
    attr_create_uint64("commitdate", lineitem_schema);
    attr_create_uint64("receipetdate", lineitem_schema);
    attr_create_string("shipinstr", 27, lineitem_schema);
    attr_create_string("shipmode", 12, lineitem_schema);
    attr_create_string("comment", 46, lineitem_schema);

    gs_frag_t *result = gs_frag_new(lineitem_schema, capacity, FIT_HOST_NSM_VM);
    gs_schema_delete(lineitem_schema);
    return result;
}

void help_fill_lineitem_table(gs_frag_t *frag, size_t num_tuples) {
    gs_tuplet_t tuplet;
    gs_tuplet_field_t field;

    for (unsigned i = 0; i < num_tuples; i++) {
        uint32_t orderkey = random() % UINT32_MAX;
        uint32_t partkey = random() % UINT32_MAX;
        uint32_t suppkey = random() % UINT32_MAX;
        int32_t linenumber = random() % INT32_MAX;
        uint32_t quantity = random() % 99;
        float extendedprice = random() % 10000;
        float discount = random() % 90 / 100.0f;
        float tax = extendedprice * 0.17f;

        char *returnflag = malloc(2);
        returnflag[0] = 'a' + (random() % 26);
        returnflag[1] = '\0';

        char *linestatus = malloc(2);
        linestatus[0] = 'a' + (random() % 26);
        linestatus[1] = '\0';

        uint64_t shipdate = random() % UINT64_MAX;
        uint64_t commitdate = random() % UINT64_MAX;
        uint64_t receipetdate = random() % UINT64_MAX;

        char *shipinstr = malloc(27),
                *shipmode = malloc(12),
                *comment = malloc(46);

        if (i == 50) {
            strcpy(shipinstr, "MERRY");
            strcpy(shipmode, "CHRISTMAS");
            strcpy(comment, "!!!");
        } else {
            for (int j = 0; j < 26; j++) {
                shipinstr[j] = 'a' + (random() % 26);
            }
            shipinstr[25] = '\0';

            for (int j = 0; j < 11; j++) {
                shipmode[j] = 'a' + (random() % 26);
            }
            shipmode[10] = '\0';

            for (int j = 0; j < 45; j++) {
                comment[j] = 'a' + (random() % 26);
            }
            comment[44] = '\0';
        }


        gs_frag_insert(&tuplet, frag, 1);
        gs_tuplet_field_open(&field, &tuplet);

        gs_tuplet_field_write(&field, &orderkey, true);
        gs_tuplet_field_write(&field, &partkey, true);
        gs_tuplet_field_write(&field, &suppkey, true);
        gs_tuplet_field_write(&field, &linenumber, true);
        gs_tuplet_field_write(&field, &quantity, true);
        gs_tuplet_field_write(&field, &extendedprice, true);
        gs_tuplet_field_write(&field, &discount, true);
        gs_tuplet_field_write(&field, &tax, true);
        gs_tuplet_field_write(&field, &returnflag, true);
        gs_tuplet_field_write(&field, &linestatus, true);
        gs_tuplet_field_write(&field, &shipdate, true);
        gs_tuplet_field_write(&field, &commitdate, true);
        gs_tuplet_field_write(&field, &receipetdate, true);
        gs_tuplet_field_write(&field, &shipinstr, true);
        gs_tuplet_field_write(&field, &shipmode, true);
        gs_tuplet_field_write(&field, &comment, true);

        free (returnflag);
        free (linestatus);
        free (shipinstr);
        free (shipmode);
        free (comment);
    }
}
