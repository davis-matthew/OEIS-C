/*
 * A302750  Number of maximum matchings in the n-path complement graph.
 * https://oeis.org/A302750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(n-k,k) *
 *            (2*(ceil(n/2)-k)-1)!! (n > 4; a(1)=a(2)=a(3)=a(4)=1)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 34, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* multiplicative binomial coefficient: binomial(n,k) */
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for 0 */
    
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* cache_size == current length (index 0 unused) */

    /* extend cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure -> treat as 0 */
        cache = new_cache;
        /* compute new entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            idx_t idx = (idx_t)i;
            seq_t val;
            if (idx <= 4) {
                val = 1;
            } else {
                idx_t half = idx / 2;
                idx_t ceil_half = (idx + 1) / 2;
                /* pre‑compute the odd double factorial for m = ceil_half */
                seq_t df = 1;
                for (idx_t t = 1; t <= ceil_half; ++t)
                    df = df * (seq_t)(2 * t - 1);
                seq_t sum = 0;
                idx_t m = ceil_half;
                for (idx_t k = 0; k <= half; ++k) {
                    seq_t term = binomial_idx(idx - k, k);
                    term = term * df;
                    if (k & 1)      /* odd k: subtract */
                        sum = sum - term;
                    else            /* even k: add */
                        sum = sum + term;
                    /* update df for next k, if any */
                    if (k < half)
                        df = df / (seq_t)(2 * m - 1);
                    --m;
                }
                val = sum;
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
