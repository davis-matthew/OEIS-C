/*
 * A327982  Partial sums of A051023, the middle column of rule-30 1-D cellular
 *          automaton, when started from a lone 1 cell.
 * https://oeis.org/A327982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051023(0) + A051023(1) + … + A051023(n) Recurrence: a(0)
 *            = 1; for n > 0, a(n) = a(n-1) + A051023(n), where A051023(n) is
 *            the middle cell of rule‑30 CA after n steps starting from a
 *            single 1. This implementation evolves the CA up to the requested
 *            index, caching intermediate sums so that ar...
 * Range:     u8 n <= 502, u16 n <= 9131, u32 n <= 9131, u64 n <= 9131,
 *            u128 n <= 9131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached partial sums a(0)…a(k).  cache_len is the number of
       already computed elements (so cache_len-1 is the largest
       index stored). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Two rows of the cellular automaton.  prev_row holds the row
       for the previous step; next_row is used to build the next
       step.  prev_len and next_len are the respective lengths. */
    static unsigned char *prev_row = NULL;
    static size_t prev_len = 0;
    static unsigned char *next_row = NULL;
    static size_t next_len = 0;

    /* ---------- initialization (first call) ---------- */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;          /* a(0) = 1 */

        prev_len = 1;
        prev_row = (unsigned char *)malloc(prev_len * sizeof(unsigned char));
        if (!prev_row) abort();
        prev_row[0] = 1;                /* single 1 cell */

        next_row = NULL;
        next_len = 0;
        cache_len = 1;                  /* we have a(0) cached */
    }

    /* ---------- extend cache up to requested n ---------- */
    if ((size_t)n >= cache_len) {
        size_t target_len = (size_t)n + 1;

        /* grow sequence cache if necessary */
        if (target_len > cache_cap) {
            size_t new_cap = target_len * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* evolve the automaton step by step */
        for (size_t step = cache_len; step <= (size_t)n; ++step) {
            /* Length of the row at this step: 2*step + 1. */
            size_t new_len = 2 * step + 1;

            /* Ensure next_row has enough space. */
            if (new_len > next_len) {
                unsigned char *tmp =
                    (unsigned char *)realloc(next_row, new_len * sizeof(unsigned char));
                if (!tmp) abort();
                next_row = tmp;
                next_len = new_len;
            }

            /* Build new row using rule‑30:
               new_row[i] = left XOR (center OR right)
               where left/right/center are cells from prev_row
               shifted by -2, -1, 0 indices respectively. */
            for (size_t i = 0; i < new_len; ++i) {
                unsigned char left   = (i >= 2 && (i - 2) < prev_len) ? prev_row[i - 2] : 0;
                unsigned char center = (i >= 1 && (i - 1) < prev_len) ? prev_row[i - 1] : 0;
                unsigned char right  = (i < prev_len) ? prev_row[i] : 0;
                next_row[i] = left ^ (center | right);
            }

            /* middle cell after this step is at index 'step'. */
            seq_t center_value = (seq_t)next_row[step];
            cache[step] = cache[step - 1] + center_value;

            /* swap rows for the next iteration */
            unsigned char *tmp_row = prev_row;
            prev_row = next_row;
            next_row = tmp_row;
            prev_len = new_len;
        }

        cache_len = target_len;
    }

    return cache[(size_t)n];
}
