/*
 * A308008  a(0)=a(1)=1; thereafter a(n+1) = 1 + a(n) + Sum_{m=1..n}
 *          binomial(2*n, 2*m-1)*a(n+1-m).
 * https://oeis.org/A308008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; for n ≥ 1, a(n+1) = 1 + a(n) + Σ_{m=1..n}
 *            binomial(2*n, 2*m‑1) * a(n+1‑m)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
static std::vector<seq_t> cache;
static idx_t computed_upto = 0;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_upto = 0;
#endif

static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    if (cache.size() <= (size_t)n)
        cache.resize(n + 1);
#else
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) abort();
        cache = new_mem;
        cache_cap = new_cap;
    }
#endif
}

seq_t a(idx_t n)
{
    if (computed_upto == 0) {
        ensure_cache(1);          /* capacity for indices 0 and 1 */
#ifdef __cplusplus
        cache.resize(2);
        cache[0] = 1;
        cache[1] = 1;
#else
        cache[0] = 1;
        cache[1] = 1;
#endif
        computed_upto = 2;
    }

    ensure_cache(n);

    while (computed_upto <= n) {
        idx_t k   = computed_upto;   /* current index to compute */
        idx_t N   = k - 1;           /* 'n' in the recurrence         */
        seq_t sum = cache[k - 1];    /* a(n) term                      */

        for (idx_t m = 1; m <= N; ++m) {
            idx_t r   = 2 * m - 1;
            seq_t bin = binomial(2 * N, r);
            sum += bin * cache[k - m];
        }
        sum += 1;                    /* constant term */
        cache[k] = sum;
        ++computed_upto;
    }

    return cache[n];
}
