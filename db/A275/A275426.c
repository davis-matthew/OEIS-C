/*
 * A275426  Number of set partitions of [n] such that eight is a multiple of
 *          each block size.
 * https://oeis.org/A275426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{s∈{1,2,4,8}, s≤n} binom(n‑1, s‑1) * a(n‑s)
 *            for n≥1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* small binomial coefficient for fixed k (k ≤ 7) */
static seq_t binom_small(idx_t N, int k)
{
    if (k == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    /* use the multiplicative formula: C(N, k) = Π_{i=1..k} (N-k+i)/i   */
    for (int i = 1; i <= k; ++i)
        res = res * ( (seq_t)(N - k + i) ) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* computed values a[0..len-1]      */
    static size_t cache_cap   = 0;   /* allocated capacity              */
    static idx_t  cache_len   = 0;   /* number of computed terms        */

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache to accommodate n+1 terms */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        /* zero the newly allocated part */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t j = cache_len; j <= n; ++j) {
        seq_t sum = (seq_t)0;

        /* block size 1 */
        if (j >= 1) {
            sum += binom_small(j - 1, 0) * cache[j - 1];
        }
        /* block size 2 */
        if (j >= 2) {
            sum += binom_small(j - 1, 1) * cache[j - 2];
        }
        /* block size 4 */
        if (j >= 4) {
            sum += binom_small(j - 1, 3) * cache[j - 4];
        }
        /* block size 8 */
        if (j >= 8) {
            sum += binom_small(j - 1, 7) * cache[j - 8];
        }
        cache[j] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
