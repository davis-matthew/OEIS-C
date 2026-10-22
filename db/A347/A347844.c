/*
 * A347844  a(n) is the number of (strict) chains of subspaces with ends 0 and
 *          (F_7)^n.
 * https://oeis.org/A347844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..n} binom_7(n,d) * a(n-d) where binom_7(n,d) =
 *            (7^n-1)(7^{n-1}-1)…(7^{n-d+1}-1) / (7^d-1)(7^{d-1}-1)…(7-1) with
 *            a(0)=1. This yields the number of strict chains of subspaces of
 *            (F_7)^n. The function caches results so that multiple calls are
 *            efficient
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for results of a(k) */
    static seq_t *cache = NULL;          /* cache[0..max_n] */
    static idx_t cache_n = 0;            /* maximum n cached so far */

    /* static cache for powers of 7: pow7[i] = 7^i, 0 <= i <= pow_cap-1 */
    static seq_t *pow7 = NULL;
    static idx_t pow_cap = 0;

    /* initialise caches on first call */
    if (cache == NULL)
    {
        cache = (seq_t*)calloc(1, sizeof(seq_t));   /* cache[0] = 0 -> will set to 1 */
        cache[0] = (seq_t)1;                       /* a(0) = 1 */
        cache_n = 0;
    }
    if (pow7 == NULL)
    {
        pow7 = (seq_t*)malloc(sizeof(seq_t));
        pow7[0] = (seq_t)1;   /* 7^0 = 1 */
        pow_cap = 1;
    }

    /* extend power cache to at least n */
    if (pow_cap <= n)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(pow7, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;            /* allocation failure unlikely */
        for (idx_t i = pow_cap; i < new_cap; ++i)
            tmp[i] = tmp[i-1] * (seq_t)7;
        pow7 = tmp;
        pow_cap = new_cap;
    }

    /* extend result cache to at least n */
    if (cache_n < n)
    {
        idx_t new_n = n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;            /* allocation failure unlikely */
        cache = tmp;

        /* compute a(k) for k = cache_n+1 .. n */
        for (idx_t k = cache_n + 1; k <= new_n; ++k)
        {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;   /* binom_7(k,0) = 1 */

            for (idx_t d = 1; d <= k; ++d)
            {
                /* update binom_7(k,d) from binom_7(k,d-1) */
                seq_t num = pow7[k - d + 1] - (seq_t)1;
                seq_t den = pow7[d] - (seq_t)1;
                binom = (binom * num) / den;   /* exact division */

                sum += binom * cache[k - d];
            }
            cache[k] = sum;
        }
        cache_n = new_n;
    }

    return cache[n];
}
