/*
 * A337556  a(0) = 1; a(n) = (1/3) * Sum_{k=1..n} binomial(n,k) * (4^k-1) *
 *          a(n-k).
 * https://oeis.org/A337556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/3) * Σ_{k=1..n} binomial(n,k) * (4^k-1) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, ensure_cache, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow4.h"
#include "../utils/ensure_cache.h"

/* Compute a(n) for any n ≥ 0, with memoisation to handle arbitrary call order. */
seq_t a(idx_t n)
{
    /* static cache array holding previously computed values */
    static seq_t *cache   = NULL;   /* pointer to the cached values */
    static size_t  cap    = 0;      /* allocated size of the cache array   */
    static idx_t   stored = 0;      /* number of terms currently in cache (len = stored) */

    /* enlarge the cache so it can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* initialise a(0) if the cache is empty */
    if (stored == 0) {
        cache[0] = 1;
        stored = 1;
    }

    /* compute any missing terms up to and including n */
    for (idx_t i = stored; i <= n; ++i) {
        /* i == 0 is already handled */
        if (i == 0) {
            continue;
        }

        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t C   = binomial(i, k);    /* C(i,k)                     */
            seq_t p4k = pow4(k);           /* 4^k                        */
            seq_t prod = (p4k - 1) * cache[i - k]; /* (4^k-1)*a[i-k]          */
            seq_t term = C * prod;           /* C * prod                   */
            sum += term;                     /* accumulate Σ */
        }
        /* division by 3 is exact per the recurrence definition */
        cache[i] = sum / 3;
    }

    stored = n + 1;   /* we now have terms 0..n cached */
    return cache[n];
}
