/*
 * A293688  Partial sums of A002251.
 * https://oeis.org/A293688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*G(n) where G(0)=0 and * G(n) = n - G(G(n-1)) for
 *            n>0
 * Range:     u8 n <= 20, u16 n <= 325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9971 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, exit */
#include "../utils/mem.h"

static seq_t get_G(idx_t n)
{
    /* Memoized cache of Hofstadter G-sequence values. */
    static seq_t *cache = NULL;   /* pointer to array of G values */
    static size_t cap   = 0;      /* current allocation size */
    static idx_t  max_  = 0;      /* highest n for which cache is filled */

    /* Ensure the cache can hold values up to n. */
    if (cap <= (size_t)n) {
        size_t new_cap = (n + 1 > cap * 2) ? (n + 1) :
                         (cap * 2 > 0 ? cap * 2 : 4);
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(1);      /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise cache if it is empty. */
    if (max_ == 0) {
        cache[0] = 0;
        if (n >= 1) cache[1] = 1;
        max_ = (n >= 1) ? 1 : 0;
    }

    /* Compute any missing terms up to n. */
    while (max_ < (idx_t)n) {
        ++max_;
        idx_t prev = max_ - 1;
        idx_t g_prev = (idx_t)cache[prev];           /* G(prev) */
        seq_t g_prev_val = cache[g_prev];            /* G(G(prev)) */
        cache[max_] = (seq_t)max_ - g_prev_val;      /* G(max_) = max_ - G(G(prev)) */
    }

    return cache[(size_t)n];
}

seq_t a(idx_t n)
{
    seq_t g = get_G(n);
    return ((seq_t)(n + 1)) * g;
}
