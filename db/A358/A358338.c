/*
 * A358338  a(n) = abs(a(n-1) - count(a(n-1))) where count(a(n-1)) is the
 *          number of times a(n-1) has appeared so far in the sequence,
 *          a(1)=0.
 * https://oeis.org/A358338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(a(n-1) - count(a(n-1))), a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *vals = NULL;
    static size_t vals_cap = 0;
    static size_t vals_len = 0;

    static uint64_t *counts = NULL;
    static size_t counts_cap = 0;
    static idx_t max_val = 0;

    if (n == 0)   /* safety – offset is 1 */
        return 0;

    /* initialise counts array with an entry for the value 0 */
    if (counts_cap == 0) {
        counts = (uint64_t*)calloc(1, sizeof(uint64_t));
        if (!counts) abort();
        counts_cap = 1;
        max_val = 0;
    }

    /* generate terms up to n (1‑based index) */
    while (vals_len < (size_t)n) {
        seq_t val;

        if (vals_len == 0) {          /* a(1) */
            val = 0;
        } else {
            seq_t prev   = vals[vals_len - 1];
            idx_t pv     = (idx_t)prev;

            uint64_t cnt_prev = 0;
            if (pv < counts_cap)
                cnt_prev = counts[pv];

            seq_t cnt_seq = (seq_t)cnt_prev;
            val = (prev >= cnt_seq) ? (prev - cnt_seq) : (cnt_seq - prev);
        }

        /* append the new term */
        if (vals_len == vals_cap) {
            size_t new_cap = vals_cap ? vals_cap * 2 : 1;
            seq_t *tmp = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            vals = tmp;
            vals_cap = new_cap;
        }
        vals[vals_len++] = val;

        /* update occurrence count for the produced value */
        idx_t v = (idx_t)val;
        if (v > max_val) {
            size_t new_cap = counts_cap;
            if (new_cap <= (size_t)v) {
                new_cap = (size_t)v + 1;
                if (new_cap < counts_cap * 2)
                    new_cap = counts_cap * 2;
            }
            uint64_t *tmpc = (uint64_t*)realloc(counts, new_cap * sizeof(uint64_t));
            if (!tmpc) abort();
            for (size_t j = counts_cap; j < new_cap; ++j)
                tmpc[j] = 0;
            counts = tmpc;
            counts_cap = new_cap;
            max_val = v;
        }
        counts[v] += 1;
    }

    return vals[(size_t)n - 1];
}
