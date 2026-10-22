/*
 * A352069  Expansion of e.g.f. 1 / (1 + log(1 - 3*x) / 3).
 * https://oeis.org/A352069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=1..n} binomial(n,k) * (k-1)! * 3^(k-1) *
 *            a(n-k) for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS A352069 sequence, using memoisation. */

#include <stdlib.h>   /* malloc, realloc, free, abort */
#include <stdint.h>   /* uint64_t */

/* External types supplied by the build system. */
extern seq_t a(idx_t n);

/* Internal cache for the sequence values. */
static seq_t *a_cache = NULL;  /* array of already computed a(n) */
static size_t a_cache_cap = 0; /* current capacity of a_cache */
static idx_t a_cache_last = 0; /* highest index already computed */

/* Ensure that a_cache can hold indices up to n.  New entries are
   initialised to 0. */
static void ensure_a_cache(idx_t n)
{
    if (a_cache_cap <= (size_t)n) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *newp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        a_cache = newp;
        for (size_t i = a_cache_cap; i < new_cap; ++i) a_cache[i] = 0;
        a_cache_cap = new_cap;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    /* Make sure the cache is large enough. */
    ensure_a_cache(n);

    /* Base case: a(0) = 1 */
    if (n == 0) {
        a_cache[0] = 1;
        a_cache_last = 0;
        return a_cache[0];
    }

    /* If already computed, return the stored value. */
    if (a_cache[n] != 0 || a_cache_last == n)
        return a_cache[n];

    /* Compute all values up to n that are still missing. */
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t c     = 1;           /* binomial(i,0) */
        seq_t fact  = 1;           /* (k-1)! initially 0! */
        seq_t pow3  = 1;           /* 3^(k-1) initially 3^0 */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient binomial(i,k) */
            c = (c * (seq_t)(i - k + 1)) / k;

            /* Compute the term for this k */
            seq_t term = c * fact;
            term = term * pow3;
            term = term * a_cache[i - k];
            sum += term;

            /* Prepare fact and pow3 for the next iteration. */
            fact *= (seq_t)k;        /* (k-1)! -> k! for next k+1 */
            pow3 *= (seq_t)3;       /* 3^(k-1) -> 3^k for next k+1 */
        }
        a_cache[i] = sum;
    }

    a_cache_last = n;
    return a_cache[n];
}
