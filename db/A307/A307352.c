/*
 * A307352  a(n) = Sum_{0<=i<=j<=k<=n} (i+j+k)!/(i!*j!*k!).
 * https://oeis.org/A307352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A307352
 *  Formula: a(n) = Sum_{0 <= i <= j <= k <= n} (i+j+k)! / (i! j! k!)
 *
 *  The implementation uses the following identity for the incremental
 *  contribution when the maximum index is increased from n-1 to n:
 *
 *     a(n) = a(n-1) + Δ(n) ,
 *
 *  where
 *
 *     Δ(n) = Σ_{t=n}^{2n}  C(t, n) * C(2t - n + 1, t - n)
 *
 *  and C(m, k) = m!/(k!(m-k)!) is the binomial coefficient.
 *
 *  Both binomial coefficients satisfy the recurrences
 *
 *     C(t+1, n) = C(t, n) * (t+1)/(t+1-n)
 *     C_{v+1} = C_v * (2v+n+3)(2v+n+2)/((v+1)(v+n+2))
 *
 *  using v = t-n.  These formulas allow Δ(n) to be computed in O(n)
 *  time with only integer arithmetic.
 *  -------------------------------------------------------------------- */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/*  compute Δ(n) for a given n  */
static inline seq_t delta_for(idx_t cur) {
    seq_t C1 = 1;          /* C(cur+v, cur)  */
    seq_t C2 = 1;          /* C(2v+cur+1, v) */
    seq_t delta = 0;

    for (idx_t v = 0; v <= cur; ++v) {
        /* add the product for this v */
        delta += C1 * C2;

        /* update C1 : C(cur+v+1, cur) = C(cur+v, cur) * (cur+v+1)/(v+1) */
        idx_t mul1 = cur + v + 1;
        idx_t div1 = v + 1;
        C1 = (C1 * mul1) / div1;

        /* update C2 :  */
        idx_t num1 = 2 * v + cur + 3;
        idx_t num2 = 2 * v + cur + 2;
        idx_t denom = (v + 1) * (v + cur + 2);
        C2 = (C2 * num1 * num2) / denom;
    }
    return delta;
}

/*  sequence generator  */
seq_t a(idx_t n) {
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    size_t old_size = cache.size();

    /* return cached value if available */
    if (n < cache.size())
        return cache[(size_t)n];

    /* enlarge the cache to n+1 elements */
    cache.resize(n + 1);

    /* initialise a(0) if the cache was empty */
    if (old_size == 0) {
        cache[0] = 1;       /* a(0) = 1 */
        old_size = 1;
    }

    /* compute successive terms */
    for (size_t cur = old_size; cur <= (size_t)n; ++cur) {
        cache[cur] = cache[cur - 1] + delta_for((idx_t)cur);
    }
    return cache[(size_t)n];

#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* return cached value if already computed */
    if (cache && n < cache_size)
        return cache[(size_t)n];

    /* enlarge the cache */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)          /* allocation failure – return 0 (undefined) */
        return 0;
    cache = new_cache;

    /* initialise a(0) if the cache was empty */
    if (old_size == 0) {
        cache[0] = 1;        /* a(0) = 1 */
        old_size = 1;
    }

    /* compute successive terms */
    for (size_t cur = old_size; cur <= (size_t)n; ++cur) {
        seq_t delta = delta_for((idx_t)cur);
        cache[cur] = cache[cur - 1] + delta;
    }
    cache_size = new_size;
    return cache[(size_t)n];
#endif
}
