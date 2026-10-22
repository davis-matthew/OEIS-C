/*
 * A386506  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} (1 + k) * k^3 *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A386506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0}^{n-1} (1+k)*k^3 *
 *            binomial(n-1,k) * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the recurrence directly, using memoisation
   to allow arbitrary order of calls.  The algorithm runs in O(n^2)
   time and uses a single dynamically allocated array for the
   previously computed terms.  All arithmetic is performed using the
   sequence type seq_t, which may be an unsigned integer type or an
   arbitrary‑precision integer class.  */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static size_t cap   = 0;       /* allocated capacity of cache   */
    static size_t used  = 0;       /* highest computed index + 1 */

    /* initial allocation */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        memset(cache, 0, cap * sizeof(seq_t));
    }

    /* grow if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        memset(cache + cap, 0, (newcap - cap) * sizeof(seq_t));
        cap = newcap;
    }

    /* already known? */
    if ((size_t)n < used) return cache[n];

    /* compute missing values up to n */
    if (used == 0) {
        cache[0] = 1;
        used = 1;
    }

    for (size_t i = used; i <= (size_t)n; ++i) {
        if (i == 0) { cache[0] = 1; continue; }

        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(i-1, 0) */

        for (size_t k = 0; k < i; ++k) {
            if (k > 0) {
                seq_t num = (seq_t)(i - 1 - (k - 1));
                seq_t den = (seq_t)k;
                binom = binom * num / den;  /* exact division */
            }

            /* weight = (1+k) * k^3 */
            seq_t weight = (seq_t)(1 + k);
            weight *= (seq_t)k * (seq_t)k * (seq_t)k;

            seq_t prod = cache[k];
            prod *= cache[i - 1 - k];
            prod *= weight;
            prod *= binom;

            sum += prod;
        }

        cache[i] = cache[i - 1] + sum;
    }

    used = (size_t)n + 1;
    return cache[n];
}
