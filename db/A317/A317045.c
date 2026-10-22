/*
 * A317045  Numbers k such that A(k+1) = A(k) + 2, where A() = A005100() are
 *          the deficient numbers.
 * https://oeis.org/A317045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 5736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* A317045: indices k such that A005100(k+1)-A005100(k)=2 (consecutive deficient numbers differ by 2). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;          /* array of sequence values                */
    static size_t cache_cap = 0;         /* capacity of the cache                 */
    static size_t cache_len = 0;         /* number of terms stored so far          */

    /* State of the running scan over integers. */
    static idx_t current_m = 1;          /* next integer to examine               */
    static idx_t prev_deficient_index = 0;   /* index (1‑based) of previous deficient */
    static idx_t prev_deficient_value = 0;   /* value of previous deficient            */
    static bool has_prev = false;             /* whether a previous deficient exists    */
    static idx_t deficient_cnt = 0;   /* count of deficient numbers seen so far */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Advance current_m until we hit a deficient number. */
        while (true) {
            seq_t sigma = sigma_of(current_m);
            if (sigma < ((seq_t)current_m * ((seq_t)2))) {
                break;   /* current_m is deficient */
            }
            ++current_m;
        }

        /* Now current_m is the next deficient number. */
        ++deficient_cnt;

        /* If the difference to the previous deficient is 2,
           the previous index satisfies the condition. */
        if (has_prev && (current_m - prev_deficient_value) == 2) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)prev_deficient_index;
        }

        /* Update state for the next iteration. */
        prev_deficient_value = current_m;
        prev_deficient_index = deficient_cnt;
        has_prev = true;
        ++current_m;
    }

    return cache[n - 1];
}
