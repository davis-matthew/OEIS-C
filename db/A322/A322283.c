/*
 * A322283  Number of permutations of [n] in which the length of every
 *          increasing run is 0 or 1 (mod 10).
 * https://oeis.org/A322283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(9,n)} (-1)^{k+1} * C(n,k) * a(n-k), with
 *            a(0) = 1. C(n,k) is the binomial coefficient
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 22, u64 n <= 32, u128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Cache for computed a(n) values.  The array stores a(n) directly. */
static seq_t *cache       = NULL;
static idx_t  cache_cap   = 0; /* size of allocated array */
static idx_t  cache_computed = 0; /* number of elements stored (0 … cache_computed-1) */

/* Resize the cache array to hold indices 0 … new_cap-1.  All
   previously computed values are copied to the new array. */
static void ensure_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        /* copy old values */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main routine to compute the sequence term a(n). */
seq_t a(idx_t n)
{
    /* Guarantee array large enough for indices 0 … n. */
    ensure_cache(n + 1);

    /* Compute any missing terms. */
    for (idx_t i = cache_computed; i <= n; ++i) {
        if (i == 0) {          /* base case a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        idx_t maxk = (i < 9 ? i : 9);
        seq_t pos = (seq_t)0; /* sum of terms with positive sign (k odd) */
        seq_t neg = (seq_t)0; /* sum of terms with negative sign (k even) */

        /* C = C(i,k) will be updated iteratively. */
        seq_t C = (seq_t)1;   /* C(i,0) = 1 */
        for (idx_t k = 1; k <= maxk; ++k) {
            /* Update binomial coefficient C(i,k) from C(i,k-1). */
            C *= (seq_t)(i - k + 1);
            C /= (seq_t)k;      /* exact division */

            /* term = C * a(i-k) */
            seq_t term = cache[i - k];
            term *= C;

            if (k & 1)       /* k odd -> positive sign */
                pos += term;
            else             /* k even -> negative sign */
                neg += term;
        }
        /* pos >= neg always holds for this sequence. */
        cache[i] = pos - neg;
    }

    cache_computed = n + 1;
    return cache[n];
}
