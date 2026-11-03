/*
 * A356346  a(n) = Sum_{k=1..n} binomial(2*k, k) * phi(k), where phi is the
 *          Euler totient function.
 * https://oeis.org/A356346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*k, k) * phi(k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Cache of previously computed values */
    static seq_t *cache = NULL;      /* cache[i] == a(i) for i>=1        */
    static idx_t  cache_cap = 0;     /* allocated capacity in cache      */
    static idx_t  cache_last = 0;    /* highest index that has been computed */

    /* Current value of the central binomial coefficient C(2k,k)
       for the last k that was processed.  Initialized to C(0,0)=1. */
    static seq_t curr_bin = (seq_t)1;

    /* If we already have the value, just return it. */
    if (n <= cache_last)
        return cache[n];

    /* Grow the cache so it can hold indices up to n. */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* Start from the last sum we have. */
    seq_t sum = (cache_last == 0) ? (seq_t)0 : cache[cache_last];

    /* Compute successive terms until we reach n. */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        /* Recurrence for the central binomial coefficient:
           C(2k,k) = C(2(k-1),k-1) * 2*(2k-1) / k
           Here 2*(2k-1) == 4*k-2. */
        seq_t mult = (seq_t)(4 * k - 2);          /* 2*(2k-1)  */
        seq_t numer = curr_bin * mult;            /* numerator */
        curr_bin = numer / (seq_t)k;              /* C(2k,k)   */

        /* Contribution of this k. */
        seq_t term = curr_bin * totient(k);

        /* Accumulate and store. */
        sum += term;
        cache[k] = sum;
    }

    cache_last = n;
    return cache[n];
}
