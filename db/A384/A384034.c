/*
 * A384034  Irregular triangle read by rows. Start with T(1,1) = 1. For each
 *          subsequent row, traverse the array so far. For each value m,
 *          insert m new values from the next unused integers immediately to
 *          the right of m. The process is repeated row by row, where each
 *          number in the array dictates how many new v...
 * https://oeis.org/A384034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Build the irregular triangle row by row.
   Start with row [1].  For each subsequent row, take the previous row;
   for each element m in that row append m consecutive unused integers
   immediately after it, forming the next row.  Flatten the rows in
   order to obtain the sequence a(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"   /* allocates array of seq_t */
#include "../utils/seq_free_array.h"       /* frees array of seq_t */

static seq_t *seq_cache = NULL;      /* cached terms */
static size_t cached_size = 0;        /* number of terms cached */
static size_t cache_cap = 0;          /* capacity of seq_cache */

static seq_t *prev_row = NULL;       /* previous row array */
static size_t prev_len = 0;           /* length of previous row */

static seq_t next_val = 0;           /* next unused integer to insert */

static bool initialized = false;

/* initialise the data structures */
static void init_sequence(void) {
    prev_len = 1;
    prev_row = seq_alloc_new_array(prev_len);
    prev_row[0] = 1;

    cache_cap = 16;
    seq_cache = seq_alloc_new_array(cache_cap);
    seq_cache[0] = 1;
    cached_size = 1;

    next_val = 2;                     /* 1 is already used */

    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_sequence();
    if (n == 0) return 0;                 /* safety for n < 1 */

    if ((idx_t)cached_size >= n) return seq_cache[n - 1];

    /* generate rows until we have at least n terms */
    while ((idx_t)cached_size < n) {
        /* sum of previous row */
        size_t sum_prev = 0;
        for (size_t i = 0; i < prev_len; ++i) {
            sum_prev += (size_t)prev_row[i];
        }

        size_t new_len = prev_len + sum_prev;
        seq_t *new_row = seq_alloc_new_array(new_len);

        size_t pos = 0;
        for (size_t i = 0; i < prev_len; ++i) {
            seq_t m = prev_row[i];
            new_row[pos++] = m;                 /* keep the original element */
            size_t cnt = (size_t)m;             /* number of insertions */
            for (size_t k = 0; k < cnt; ++k) {
                new_row[pos++] = next_val;      /* insert consecutive unused integer */
                next_val += 1;
            }
        }

        /* grow cache if needed */
        if (cache_cap < cached_size + new_len) {
            size_t new_cap = cache_cap > 0 ? cache_cap : 16;
            while (new_cap < cached_size + new_len) new_cap <<= 1;
            seq_t *tmp = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < cached_size; ++i) tmp[i] = seq_cache[i];
            seq_free_array(seq_cache);
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        /* append the new row to the cache */
        for (size_t i = 0; i < new_len; ++i) {
            seq_cache[cached_size + i] = new_row[i];
        }
        cached_size += new_len;

        /* free previous row and set new_row as current */
        seq_free_array(prev_row);
        prev_row = new_row;
        prev_len = new_len;
    }

    return seq_cache[n - 1];
}
