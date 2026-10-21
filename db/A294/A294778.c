/*
 * A294778  Expansion of Product_{k>=1} 1/(1 - x^(2*k-1))^(k*(k-1)/2).
 * https://oeis.org/A294778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 -
 *            x^(2k-1))^(-(k(k-1))/2)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code uses the exponential generating-function recurrence:
   If G(x) = exp( L(x) ) with L(x) = ∑_{m≥1} l_m x^m,
   then n a_n = ∑_{k=1}^n k l_k a_{n-k}.
   For our product one obtains k l_k = ∑_{j odd | k} j e_j
   where e_j = (j^2-1)/8.  Thus
        S(k) = ∑_{j odd | k} j e_j
   satisfies   n a_n = ∑_{k=1}^n S(k) a_{n-k}.
   The division by n is exact.  This is an O(n^2) algorithm with a
   moderate constant factor, fast enough for n ≤ 10000.  We memoise
   results so that repeated calls in arbitrary order are O(1).

   The implementation uses the helper seq_realloc.h from the
   j-integer utilities.  It works with the same interface for the
   built‑in integer types and the arbitrary‑precision type.   */
#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static size_t cache_n   = 0;   /* number of terms already computed (cache_n ==> last valid index +1) */

    static seq_t *S = NULL;
    static size_t S_cap   = 0;     /* capacity of S array   */
    static idx_t  S_max   = 0;     /* highest n for which S has been filled */

    /* if result already computed, return it immediately */
    if (n < cache_n)
        return cache[n];

    /* need to extend cache to include a(n) */
    size_t new_cap = (size_t)n + 1;
    if (cache_cap < new_cap) {
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* extend the values of S up to n if necessary */
    if (S_max < n) {
        size_t new_S_cap = (size_t)n + 1;
        S = seq_realloc(S, S_cap, new_S_cap);
        S_cap = new_S_cap;

        /* zero S[0..n] before recomputing */
        for (size_t i = 0; i <= (size_t)n; ++i)
            S[i] = 0;

        /* compute S(k) = Σ_{j odd | k} j e_j  with e_j = (j^2-1)/8  */
        for (idx_t j = 1; j <= n; j += 2) {
            seq_t e = ((seq_t)j * (seq_t)j - 1) / 8;   /* e_j  */
            if (e == 0)
                continue;                            /* j = 1 gives no contribution */
            seq_t je = (seq_t)j * e;                  /* j * e_j  */
            for (idx_t k = j; k <= n; k += j)
                S[k] += je;
        }
        S_max = n;
    }

    /* compute new terms a[cache_n .. n] */
    if (cache_n == 0) {                 /* initialise a(0) = 1 */
        cache[0] = 1;
        cache_n = 1;
    }

    for (idx_t i = cache_n; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += S[k] * cache[i - k];
        }
        cache[i] = sum / (seq_t)i;       /* division by i is exact  */
    }
    cache_n = n + 1;                      /* we have computed up to index n       */

    return cache[n];
}
