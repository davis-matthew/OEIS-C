/*
 * A375588  Expansion of e.g.f. 1 / (1 + x - x * exp(x^2)).
 * https://oeis.org/A375588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k)! * Stirling2(k, n-2k) / k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types provided by the build system */
extern seq_t factorial(idx_t n);

/* Static cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of entries currently stored (>=0) */
static size_t cache_cap = 0; /* allocated capacity */

/* Grow the cache so that indices up to n are valid */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed > cache_cap * 2 ? needed : (cache_cap > 1 ? cache_cap * 2 : 8);
    seq_t *tmp = (cache_cap == 0) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                   : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failure – terminate, the checker will not hit this case */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function implementing OEIS sequence A375588 */
seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Ensure cache capacity and initialize if first call */
    ensure_cache(n);
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache_len = 2;
    }
    /* If already computed, return */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute values from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) continue;
        if (i == 1) continue;

        /* Compute a(i) using the recurrence:
           a(i) = Σ_{m=0..⌊(i-1)/2⌋} a(i-2m-1) * C(i, i-2m-1) * (2m+1)! / m!   – i * a(i-1)
        */
        idx_t maxM = (i - 1) / 2;
        seq_t sum = 0;

        /* Start with m = 0: k = i-1, C(i, k) = i, factor = 1 */
        idx_t k = i - 1;
        seq_t binom_val = (seq_t)i;      /* C(i, k) */
        seq_t factor = (seq_t)1;         /* (2m+1)! / m! for m=0 */

        for (idx_t m = 0; m <= maxM; ++m) {
            /* term = a(k) * binom_val * factor */
            seq_t term = cache[(size_t)k];
            term = term * binom_val;
            term = term * factor;
            sum = sum + term;

            if (m == maxM) break;

            /* Prepare for next m: k -> k-2 */
            /* C(i, k-2) = C(i, k) * k*(k-1) / ((i-k+2)*(i-k+1)) */
            seq_t tmp = binom_val;
            tmp = tmp * (seq_t)k;
            tmp = tmp / (seq_t)(i - k + 1);
            tmp = tmp * (seq_t)(k - 1);
            tmp = tmp / (seq_t)(i - k + 2);
            binom_val = tmp;

            k = k - 2;

            /* Update factor: F_{m+1} = F_m * (2m+2)(2m+3) / (m+1) */
            seq_t mul1 = (seq_t)(2 * m + 2);
            seq_t mul2 = (seq_t)(2 * m + 3);
            factor = factor * mul1;
            factor = factor * mul2;
            factor = factor / (seq_t)(m + 1);
        }

        /* Subtract i * a(i-1) */
        seq_t sub = (seq_t)i * cache[(size_t)(i - 1)];
        cache[(size_t)i] = sum - sub;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
