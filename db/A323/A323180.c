/*
 * A323180  Row sums of A323179.
 * https://oeis.org/A323180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of the modified Rule 30 cellular automaton
 *            where any positive neighbour value is treated as TRUE; a(0) = 1,
 *            a(1) = 3, a(2) = 4, …
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Rule‑30 truth table for 3‑bit patterns (left × 4 + center × 2 + right). */
static const unsigned char R30[8] = {
    0, /* 000 */
    1, /* 001 */
    1, /* 010 */
    1, /* 011 */
    1, /* 100 */
    0, /* 101 */
    0, /* 110 */
    0  /* 111 */
};

/* ---------------------------------------------------------------------
   Allocation helpers that work in both C and C++.
--------------------------------------------------------------------- */
#ifdef __cplusplus
static inline seq_t *allocate_seq_array(size_t n) { return new seq_t[n]; }
static inline void free_seq_array(seq_t *p) { delete [] p; }
#else
static inline seq_t *allocate_seq_array(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static inline void free_seq_array(seq_t *p) { free(p); }
#endif

/* ---------------------------------------------------------------------
   Cached data: we keep two rows and a sum cache that grows
   only when required.
--------------------------------------------------------------------- */
static seq_t *row_sum_cache = NULL;    /* a(0)…a(max_cached)          */
static size_t cache_cap     = 0;        /* capacity of row_sum_cache     */
static seq_t *prev_row      = NULL;    /* previous CA row              */
static size_t prev_len      = 0;        /* length of prev_row            */
static idx_t  max_cached    = (idx_t)-1;/* indices up to max_cached ready */

/* Ensure that rows 0 … n are computed and a(n) is cached. */
static void ensure_row(idx_t n)
{
    /* Nothing to do if n is already cached. */
    if (max_cached != (idx_t)-1 && n <= max_cached)
        return;

    /* First time: initialise row 0. */
    if (prev_row == NULL) {
        prev_len = 1;
        prev_row = allocate_seq_array(prev_len);
        prev_row[0] = (seq_t)1;

        /* Allocate cache for a(0). */
        cache_cap = 1;
        row_sum_cache = allocate_seq_array(cache_cap);
        row_sum_cache[0] = (seq_t)1;
        max_cached = 0;
    }

    /* Grow the sum cache if needed. */
    if (cache_cap <= (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;

        seq_t *tmp = allocate_seq_array(new_cap);
        for (size_t i = 0; i <= max_cached; ++i)
            tmp[i] = row_sum_cache[i];
        free_seq_array(row_sum_cache);
        row_sum_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute successive rows until n is reached. */
    for (idx_t cur = max_cached + 1; cur <= n; ++cur) {
        size_t new_len = prev_len + 2;
        seq_t *new_row = allocate_seq_array(new_len);

        seq_t row_sum = 0;

        for (size_t k = 0; k < new_len; ++k) {
            seq_t a = (k >= 2 && k - 2 < prev_len) ? prev_row[k - 2] : (seq_t)0;
            seq_t b = (k >= 1 && k - 1 < prev_len) ? prev_row[k - 1] : (seq_t)0;
            seq_t c = (k < prev_len) ? prev_row[k] : (seq_t)0;

            unsigned pat = ((a > (seq_t)0) << 2) |
                           ((b > (seq_t)0) << 1) |
                           (c > (seq_t)0);

            if (R30[pat]) {
                seq_t val = a + b + c;
                new_row[k] = val;
                row_sum += val;
            } else {
                new_row[k] = (seq_t)0;
            }
        }

        free_seq_array(prev_row);
        prev_row = new_row;
        prev_len = new_len;

        row_sum_cache[cur] = row_sum;
        max_cached = cur;
    }
}

/* ---------------------------------------------------------------------
   Public entry point.
--------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_row(n);
    return row_sum_cache[n];
}
