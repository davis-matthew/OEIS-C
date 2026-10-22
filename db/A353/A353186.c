/*
 * A353186  Expansion of e.g.f. 1/(1 - Sum_{k>=1} d(k) * x^k / k), where d(n)
 *          = number of divisors of n (A000005).
 * https://oeis.org/A353186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) = Sum_{k=1..n} d(k) * (k-1)! * binomial(n,k) *
 *            a(n-k) where d(k) is the number of divisors of k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* divisor count: number of positive divisors of n */
static uint32_t divisor_count(uint32_t n)
{
    uint32_t cnt = 0;
    uint32_t i;
    for (i = 1; (uint64_t)i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            cnt += 2;
            if ((uint64_t)i * i == n) cnt -= 1;
        }
    }
    return cnt;
}

/* sequence computation */
seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;   /* a[0..N]                  */
    static seq_t *b_cache = NULL;   /* b[k] = d(k)*(k-1)!       */
    static seq_t *fact_cache = NULL;/* factorial[k] = k!        */
    static size_t cache_limit = 0;  /* arrays size (max index+1) */

    if (n == 0u) return (seq_t)1;   /* a(0) = 1 */

    /* Expand caches if needed */
    if ((size_t)n + 1 > cache_limit)
    {
        size_t new_limit = (size_t)n + 1;
        /* Reallocate arrays */
        a_cache    = (seq_t *)realloc(a_cache, new_limit * sizeof(seq_t));
        b_cache    = (seq_t *)realloc(b_cache, new_limit * sizeof(seq_t));
        fact_cache = (seq_t *)realloc(fact_cache, new_limit * sizeof(seq_t));

        /* factorials */
        if (cache_limit == 0)
            fact_cache[0] = (seq_t)1;
        for (size_t i = cache_limit; i < new_limit; ++i)
        {
            if (i == 0) continue;
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        }

        /* b(k) = d(k)*(k-1)!  for k>=1 */
        if (cache_limit == 0) b_cache[0] = (seq_t)0; /* unused */
        size_t start_k = (cache_limit == 0) ? 1 : cache_limit;
        for (size_t k = start_k; k < new_limit; ++k)
        {
            uint32_t d = divisor_count((uint32_t)k);
            b_cache[k] = (seq_t)d * fact_cache[k-1];
        }

        /* a[0] is known */
        if (cache_limit == 0)
            a_cache[0] = (seq_t)1;

        /* compute a values from old limit to new_limit-1 */
        size_t start_i = (cache_limit > 0) ? cache_limit : 1;
        for (size_t i = start_i; i < new_limit; ++i)
        {
            seq_t sum = 0;
            seq_t binom = 1;  /* binom(i,0) */
            for (size_t k = 1; k <= i; ++k)
            {
                /* binom(i,k) = binom(i,k-1) * (i-k+1) / k */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
                seq_t term = binom * a_cache[i - k];
                term = term * b_cache[k];
                sum += term;
            }
            a_cache[i] = sum;
        }

        cache_limit = new_limit;
    }

    return a_cache[n];
}
