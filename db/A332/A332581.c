/*
 * A332581  a(0)=0, a(1)=1; for n>1, a(n) = max(sum0,sum1) mod n, where sum0
 *          is the sum of all previous even terms, sum1 is the sum of all
 *          previous odd terms.
 * https://oeis.org/A332581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1006, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n>1, a(n)=max(sum0,sum1) mod n,
   where sum0 is the sum of all previous even terms and sum1 the sum
   of all previous odd terms. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caching of computed values and running sums of even/odd terms */
    static seq_t *cache   = NULL; /* array of a(k) values */
    static size_t cap     = 0;    /* current capacity of cache */
    static size_t sz      = 0;    /* number of values currently stored (cache[0..sz-1]) */
    static seq_t sum_even = 0;    /* sum of even a(k) for k<sz */
    static seq_t sum_odd  = 0;    /* sum of odd  a(k) for k<sz */

    /* Ensure the array is large enough for indices 0..n */
    size_t need_size = (size_t)n + 1;
    if (cap < need_size) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < need_size)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)0;
        } else if (i == 1) {
            val = (seq_t)1;
        } else {
            seq_t mx = sum_even > sum_odd ? sum_even : sum_odd;
            val = mx % (seq_t)i;               /* a(i) = mx mod i */
        }
        cache[i] = val;
        /* Update running sums of even/odd a(k) */
        if ((val & (seq_t)1) == (seq_t)0)
            sum_even += val;
        else
            sum_odd  += val;
        sz = (size_t)i + 1;
    }

    return cache[n];
}
