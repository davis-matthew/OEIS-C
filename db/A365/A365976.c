/*
 * A365976  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(4*k+1) / (4*k+1) ).
 * https://oeis.org/A365976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/4)} (4*k)! * binomial(n,4*k+1) *
 *            a(n-4*k-1) with a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *a_cache = NULL;   /* array of a values */
static size_t a_cache_size = 0; /* current capacity of a_cache */

/* Ensure that the cache has capacity for indices 0..n, zero‑initialising new slots. */
static void
ensure_a_cache(idx_t n)
{
    if (a_cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                     /* out of memory */
        /* zero‑initialise the new slots */
        for (size_t i = a_cache_size; i < new_size; ++i)
            tmp[i] = 0;
        a_cache = tmp;
        a_cache_size = new_size;
    }
}

/* Return a(n) with memoisation */
seq_t a(idx_t n)
{
    ensure_a_cache(n);

    /* base case */
    if (n == 0) {
        if (a_cache[0] == 0)
            a_cache[0] = 1;
        return a_cache[0];
    }

    /* return cached value if already computed */
    if (a_cache[n] != 0)
        return a_cache[n];

    idx_t maxk = (n - 1) / 4;               /* highest k such that 4k+1 <= n */
    seq_t result = 0;

    /* current factorial (4*k)! and binomial C(n,4*k+1) */
    seq_t fac = 1;          /* (4*0)! = 1 */
    seq_t binom = (seq_t)n; /* C(n,1) = n */
    idx_t curr_m = 1;       /* current exponent 4*k+1 */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* term = (4*k)! * C(n,4*k+1) * a(n-4*k-1) */
        seq_t sub = a(n - curr_m);          /* recursion ensures sub is ready */
        seq_t term = fac * binom;
        term *= sub;
        result += term;

        /* prepare fac and binom for the next k (if any) */
        if (k < maxk) {
            /* update factorial: multiply by (4*k+1)*(4*k+2)*(4*k+3)*(4*k+4) */
            seq_t mul = 1;
            for (int t = 1; t <= 4; ++t)
                mul *= (4 * k + t);
            fac *= mul;

            /* update binomial: from C(n, curr_m) to C(n, curr_m+4) */
            for (int t = 1; t <= 4; ++t) {
                seq_t num = (seq_t)(n - (curr_m + t - 1));
                seq_t den = (seq_t)(curr_m + t);
                binom = binom * num / den;    /* exact division */
            }
            curr_m += 4;
        }
    }

    a_cache[n] = result;
    return result;
}
