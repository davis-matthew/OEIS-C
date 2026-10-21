/*
 * A265842  Expansion of Product_{k>=1} (1 + k^5*x^k).
 * https://oeis.org/A265842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in ∏_{k≥1} (1 + k^5 x^k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 28, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e.  a(0)=1,  a(n)=∑_{k=1}^{n} k^5 * a(n−k), with each part used at most once */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;     /* array of precalculated a(n) */
static idx_t cache_cap = 0;     /* number of elements stored (equals current size) */

/* recompute values up to (and including) n, if necessary */
static void recompute(idx_t n)
{
    if (cache_cap > n) return;               /* already have required data */

    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) return;                       /* allocation failed – fall back to old cache */
    cache = tmp;

    /* initialise all entries to zero */
    for (idx_t i = 0; i < new_cap; ++i)
        cache[i] = (seq_t)0;

    cache[0] = (seq_t)1;                    /* a(0) = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t k2   = kk * kk;               /* k^2      */
        seq_t k3   = k2 * kk;               /* k^3      */
        seq_t k5   = k2 * k3;               /* k^5      */

        /* add contributions of part k to all sums j ≥ k */
        for (idx_t j = n; j >= k; --j) {
            seq_t prod = cache[j - k] * k5;
            cache[j] += prod;
            if (j == k) break;              /* guard against unsigned underflow */
        }
    }

    cache_cap = new_cap;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    recompute(n);                          /* ensure cache has a(n) */
    return cache[n];
}
