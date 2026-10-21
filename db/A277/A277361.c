/*
 * A277361  a(n) = Sum_{k=0..n} k^3 * binomial(n-k, k).
 * https://oeis.org/A277361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^3 * binomial(n-k, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache static data for repeated calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* number of entries allocated */
    static idx_t cached_up_to = 0;    /* last index whose value is stored */

    /* handle small indices directly */
    if (n < 2)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* first use: allocate and initialise */
    if (cache == NULL) {
        cache_size = (size_t)(n + 1);
        if (cache_size < 8)
            cache_size = 8;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                 /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cached_up_to = 2;
    }

    /* grow cache if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = cache_size ? cache_size * 2 : 8;
        if (new_size < (size_t)n + 1)
            new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;                 /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* if already computed, return */
    if (cached_up_to >= n)
        return cache[n];

    /* compute missing values using the recurrence */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)0;
            continue;
        }
        if (i == 2) {
            cache[i] = (seq_t)1;
            continue;
        }

        /* indices for recurrence: n = i-1 */
        idx_t n_idx = i - 1;
        idx_t n_minus1_idx = i - 2;

        seq_t a_n = cache[n_idx];           /* a[n]   */
        seq_t a_nm1 = cache[n_minus1_idx];   /* a[n-1] */

        seq_t n_seq = (seq_t)n_idx;
        seq_t n_sq = n_seq * n_seq;               /* n^2 */
        seq_t n_cu = n_sq * n_seq;                /* n^3 */
        seq_t n_4 = n_cu * n_seq;                 /* n^4 */
        seq_t n_5 = n_4 * n_seq;                  /* n^5 */

        /* numerator components */
        seq_t poly1 = n_5 + (seq_t)3 * n_4 - (seq_t)5 * n_cu
                      - (seq_t)5 * n_sq - n_seq + (seq_t)2;
        seq_t term1 = poly1 * a_n;

        seq_t n_plus1 = n_seq + (seq_t)1;
        seq_t n_plus1_sq = n_plus1 * n_plus1;
        seq_t inner = n_cu + (seq_t)4 * n_sq + n_seq - (seq_t)1;
        seq_t term2 = n_plus1_sq * inner * a_nm1;

        seq_t numerator = term1 + term2;

        /* denominator */
        seq_t denom_poly = n_4 - (seq_t)5 * n_sq + (seq_t)5 * n_seq
                           - (seq_t)1;
        seq_t denom = n_seq * denom_poly;

        cache[i] = numerator / denom;
    }

    cached_up_to = n;
    return cache[n];
}
