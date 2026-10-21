/*
 * A240936  Number of ways to partition the (vertex) set {1,2,...,n} into any
 *          number of classes and then select some unordered pairs (edges)
 *          <a,b> such that a and b are in distinct classes of the partition.
 * https://oeis.org/A240936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A240936: a(n) = sum_{t=1..n} C(n-1,t-1)*2^{t*(n-t)}*a(n-t),  a(0)=1 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* cache: a(n) values already computed */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* capacity of cache */
static idx_t cached_max = 0;    /* largest n computed so far */

/* ensure cache has room for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        abort(); /* allocation failure */

    if (cache) {
        memcpy(new_arr, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }

    cache = new_arr;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* guarantee that we have a(0) ready */
    ensure_cache(n);
    if (cached_max == 0 && cache[0] == 0)
        cache[0] = 1;               /* a(0) = 1 */

    if (n == 0)
        return cache[0];

    /* compute values up to n on demand */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        idx_t N = k - 1;           /* N = n-1 for this iteration */
        seq_t binom = 1;           /* C(N,0) */

        for (idx_t t = 1; t <= k; ++t) {
            idx_t exp = t * (k - t);          /* t*(n-t) */
            seq_t term = binom;
            term *= cache[k - t];
            term *= pow2(exp);
            sum += term;

            if (t < k) {
                /* update binom to C(N, t) */
                seq_t numerator = binom * (seq_t)(N - (t - 1));
                binom = numerator / (seq_t)t;
            }
        }
        cache[k] = sum;
    }

    cached_max = n;
    return cache[n];
}
