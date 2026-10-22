/*
 * A368635  Expansion of g.f. A(x) satisfying A(x) = 1 + 3*x*A(x)^2 -
 *          2*x*A(-x)^2.
 * https://oeis.org/A368635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   For n ≥ 1 let S_k = Σ_{i=0}^{k} a[i] * a[k-i].
   Then
       a(n) =  S_{n-1}             if n is odd
       a(n) = 5 * S_{n-1}          if n is even
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* Stored sequence values               */
static idx_t  cache_len = 0;         /* Index of the largest computed term   */
static size_t cache_cap = 0;         /* Allocated capacity (in seq_t units)   */

/* Grow the cache array so that it can hold at least (need + 1) elements   */
static void grow_cache(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;
    size_t new_cap = ((size_t)need + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – terminate the program safely            */
        exit(1);
    }
    cache    = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 8;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 0;         /* The last computed index is 0 at this point */
    }

    /* Return a cached value if we already know it */
    if (n <= cache_len)
        return cache[n];

    /* Compute successive terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Compute S_{k-1} = Σ_{i=0}^{k-1} a[i] * a[k-1-i] */
        seq_t S = 0;
        for (idx_t i = 0; i <= k - 1; ++i) {
            S += cache[i] * cache[k - 1 - i];
        }

        seq_t term = ((k & 1u) == 0) ? (seq_t)5 * S : S;   /* k even → 5S, else S */

        grow_cache(k);
        cache[k] = term;
        cache_len = k;
    }

    return cache[n];
}
