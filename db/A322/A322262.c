/*
 * A322262  Number of permutations of [n] in which the length of every
 *          increasing run is 0 or 1 (mod 6).
 * https://oeis.org/A322262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(5,n)} a(n-k) * (-1)^{k+1} * C(n,k) with a(0)
 *            = 1. C(n,k) is the binomial coefficient n choose k
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 17, u64 n <= 27, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Memoisation cache for computed terms */
static seq_t *cache = NULL;      /* cached values a(0) .. a(cache_size-1) */
static idx_t cache_cap = 0;      /* allocated capacity of cache (>= cache_size) */
static idx_t cache_size = 0;     /* number of computed terms */

/* Compute binomial coefficient C(n,k) for 0 <= k <= 5  (k! fits in a word). */
static inline seq_t binom_nk(idx_t n, idx_t k)
{
    if (k == 0) return 1;
    seq_t r = 1;
    for (idx_t t = 0; t < k; ++t)
    {
        r = r * (seq_t)(n - t);
        r = r / (t + 1);
    }
    return r;
}

/* Grow the cache to hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;       /* allocation failure – nothing can be done */
        cache[0] = 1;             /* a(0) = 1 */
        cache_size = 1;
    }

    if (n + 1 > cache_cap)
    {
        idx_t new_cap = cache_cap;
        while (n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;        /* allocation failure – keep old cache */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t plus = 0, minus = 0;
        idx_t maxk = (i < 5) ? i : 5;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            seq_t bin = binom_nk(i, k);
            seq_t term = cache[i - k] * bin;
            if ((k & 1) == 0)          /* even k -> subtract  (sign = –) */
                minus += term;
            else                       /* odd k  -> add     (sign = +) */
                plus += term;
        }
        /* plus ≥ minus for all n, so this subtraction is safe */
        cache[i] = plus - minus;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 0;              /* safety for underflow */
    ensure_cache(n);
    return cache[n];
}
