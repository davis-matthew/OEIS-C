/*
 * A251218  Number of (n+1)X(7+1) 0..1 arrays with no 2X2 subblock having zero
 *          or two 1s.
 * https://oeis.org/A251218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×8 binary matrices with no 2×2 subblock *
 *            having 0 or 2 ones
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   allowed_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allowed_block.h"

/* -------------------------------------------------------------------- */
/* Dynamic allocation helpers that work both in C and C++               */
/* -------------------------------------------------------------------- */
static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* -------------------------------------------------------------------- */
/* Transition table between rows                                          */
/* -------------------------------------------------------------------- */
static bool Transition[256][256];
static bool initialized = false;

/* -------------------------------------------------------------------- */
/* DP vectors and cache                                                     */
/* -------------------------------------------------------------------- */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static seq_t *table = NULL;      /* table[n] holds a(n) for n>=1          */
static idx_t current_step = 0;   /* number of transitions computed so far */
static size_t table_size = 0;    /* allocated size of table              */

/* -------------------------------------------------------------------- */
/* Helper to compute whether a 2x2 subblock is allowed                  */

/* -------------------------------------------------------------------- */
/* Build the transition matrix once                                       */
/* -------------------------------------------------------------------- */
static void init(void) {
    if (initialized) return;

    /* Build the 256×256 transition matrix */
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            bool ok = true;
            for (int j = 0; j < 7; ++j) {
                int a1 = (a >> j) & 1;
                int a2 = (a >> (j + 1)) & 1;
                int b1 = (b >> j) & 1;
                int b2 = (b >> (j + 1)) & 1;
                if (!allowed_block(a1, a2, b1, b2)) {
                    ok = false;
                    break;
                }
            }
            Transition[a][b] = ok;
        }
    }

    /* Allocate DP vectors */
    dp_cur = alloc_seq_array(256);
    dp_next = alloc_seq_array(256);
    for (int i = 0; i < 256; ++i) dp_cur[i] = 1;   /* all first rows allowed */

    /* Allocate cache for a(1)..*/
    table = alloc_seq_array(1);  /* table[0] unused */
    table[0] = 0;
    table_size = 1;
    current_step = 0;
    initialized = true;
}

/* -------------------------------------------------------------------- */
/* Main exported function                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    init();

    if (n >= (idx_t)table_size) {
        /* Grow the cache to accommodate up to n */
        size_t new_size = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *new_table = new seq_t[new_size];
        for (size_t i = 0; i < table_size; ++i) new_table[i] = table[i];
        delete[] table;
#else
        seq_t *new_table = (seq_t *)realloc(table, new_size * sizeof(seq_t));
#endif
        table = new_table;
        table_size = new_size;
    }

    /* Extend DP up to requested n */
    for (idx_t step = current_step + 1; step <= n; ++step) {
        /* Zero dp_next */
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;

        /* dp_next[b] = sum_a dp_cur[a] * Transition[a][b] */
        for (int a = 0; a < 256; ++a) {
            seq_t val = dp_cur[a];
            if (!val) continue;
            for (int b = 0; b < 256; ++b) {
                if (Transition[a][b]) dp_next[b] += val;
            }
        }

        /* Sum all entries to obtain a(step) */
        seq_t sum = 0;
        for (int b = 0; b < 256; ++b) sum += dp_next[b];
        table[step] = sum;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        current_step = step;
    }

    return table[n];
}
