/*
 * A349923  Dirichlet g.f.: Product_{k>=2} (1 + 2 * k^(-s)).
 * https://oeis.org/A349923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A349923.  
   For n ≥ 1: a(1) = 1.  
   For n > 1, a(n) is the sum of 2^|S| over all subsets S of the set of integers ≥2
   whose product is n, equivalently a(n) is obtained by iterating each integer k≥2
   once and adding its contribution 2⋅a(n/k) only when k divides n.  This
   dynamic‑programming computation directly realizes the Dirichlet product
   ∏_{k≥2}(1+2k^{-s}) without storing the factorisation of every n. */

#include <stdlib.h>

/* External prototypes provided by the build system */
extern seq_t a(idx_t n);

/* Static cache for computed values */
static seq_t *cache = NULL;     /* indices 1 … cache_size are filled */
static idx_t cache_size = 0;    /* maximum n for which cache is valid */

/* Recomputes all values up to 'need' from scratch */
static void recompute(idx_t need)
{
    /* Resize cache */
    cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
    /* Base case a(1) = 1 */
    cache[1] = (seq_t)1;
    /* Initialise rest to zero */
    for (idx_t i = 2; i <= need; ++i)
        cache[i] = 0;

    /* Dynamic programming: for each k, update all multiples m ≥ k */
    for (idx_t k = 2; k <= need; ++k) {
        for (idx_t m = need; m >= k; --m) {
            if (m % k == 0)
                cache[m] = cache[m] + (seq_t)2 * cache[m / k];
        }
    }
    cache_size = need;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have precomputed up to n */
    if (cache_size < n)
        recompute(n);
    return cache[n];
}
