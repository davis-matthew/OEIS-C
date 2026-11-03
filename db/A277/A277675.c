/*
 * A277675  Numbers k such that d(k+2) = d(k+1), where d(m) is the number of
 *          digits in the base-m representation of m!.
 * https://oeis.org/A277675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that d(k+2) = d(k+1), where
   d(m) = floor( ln(m!)/ln(m) ) + 1 (number of digits of m! in base m). */

#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

/* Return floor( ln(m!)/ln(m) ) + 1.
   The function keeps a running sum of ln(i) for i = 1..cur_m,
   so each call increases cur_m only if necessary. */
static idx_t digits_factorial_in_base(idx_t m)
{
    static idx_t cur_m = 0;
    static long double sum_ln = 0.0L;

    while (cur_m < m) {
        ++cur_m;
        sum_ln += logl((long double)cur_m);
    }

    /* ln(m!)/ln(m)   */
    long double val = sum_ln / logl((long double)m);
    /* tiny epsilon to guard against rounding at an integer boundary */
    const long double eps = 1e-12L;
    return (idx_t)(floorl(val + eps)) + 1;
}

/* Return the nth term of the sequence.
   The implementation lazily generates terms and remembers
   all terms seen so far in a dynamically allocated array. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_k = 1;  /* next k to test */

    if (n < 1) return 0;  /* undefined for n < 1 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t d1 = digits_factorial_in_base(next_k + 1);
        idx_t d2 = digits_factorial_in_base(next_k + 2);

        if (d1 == d2) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                void *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();            /* allocation failure */
                cache = (seq_t *)tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
