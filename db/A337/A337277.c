/*
 * A337277  Stern's triangle read by rows.
 * https://oeis.org/A337277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Stern's triangle read by rows. The triangle is generated
 *            recursively: row0 = [1] row_{k+1} = [1] || row_k || (row_k[i] +
 *            row_k[i+1] for all i) || [1] The sequence a(n) is obtained by
 *            reading all rows left‑to‑right
 * Range:     u8 n <= 9958, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state – cached values and current row */
    static seq_t *cache = NULL;      /* flattened rows so far */
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t *row  = NULL;      /* previous row */
    static size_t row_len = 0, row_cap = 0;
    static bool initialized = false;

    /* first call: initialise the cache and the first row */
    if (!initialized) {
        /* initial cache capacity */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        /* first row [1] */
        row_cap = 8;
        row = (seq_t *)malloc(row_cap * sizeof(seq_t));
        row_len = 1;
        row[0] = (seq_t)1;
        cache[cache_len++] = (seq_t)1;
        initialized = true;
    }

    /* generate rows until we have at least n+1 flattened elements */
    while (cache_len <= (size_t)n) {
        /* new row length = 2*row_len + 1 */
        size_t newlen = 2 * row_len + 1;
        seq_t *newrow = (seq_t *)malloc(newlen * sizeof(seq_t));

        size_t idx = 0;
        newrow[idx++] = (seq_t)1;                  /* leading 1 */

        for (size_t i = 0; i < row_len; ++i) {
            newrow[idx++] = row[i];                 /* copy element */
            if (i + 1 < row_len)                    /* insert sum */
                newrow[idx++] = row[i] + row[i + 1];
        }

        newrow[idx++] = (seq_t)1;                  /* trailing 1 */
        /* idx now equals newlen */

        /* enlarge cache if necessary */
        if (cache_len + newlen > cache_cap) {
            size_t new_cap = cache_cap;
            while (cache_len + newlen > new_cap)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* append new row to cache */
        for (size_t i = 0; i < newlen; ++i)
            cache[cache_len++] = newrow[i];

        /* prepare for next iteration */
        free(row);
        row = newrow;
        row_len = newlen;
    }

    return cache[(size_t)n];
}
