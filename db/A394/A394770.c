/*
 * A394770  Smallest base b for which the average of the digits of n base b
 *          divided by b is the largest for any base b <= n.
 * https://oeis.org/A394770
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = argmax_{2 <= b <= n} * (digit_sum(n, b) / (digit_count(n,
 *            b) * b)) * i.e. the smallest base that maximizes the ratio of
 *            the average * of the base‑b digits of n divided by the base
 * Range:     u8 n <= 1341, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_and_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum_and_count.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                     /* undefined, safe value */

    /* cache of already computed results */
    static idx_t *cache = NULL;              /* cached best base for each n */
    static size_t cache_cap = 0;

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = ((size_t)n + 1) * 2;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        if (!tmp)                              /* allocation failed */
            return (seq_t)0;
        /* initialise new slots to 0 (meaning 'not yet computed') */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if available */
    if (cache[n] != 0)
        return (seq_t)cache[n];

    /* compute best base for this n */
    idx_t best_b = 0;
    seq_t best_num = 0;                 /* numerator of the ratio */
    seq_t best_den = 1;                 /* denominator of the ratio */

    for (idx_t b = 2; b <= n; ++b)
    {
        seq_t sum_digits;
        unsigned int digit_cnt;
        digit_sum_and_count(n, b, &sum_digits, &digit_cnt);

        seq_t denom = (seq_t)digit_cnt * (seq_t)b;

        /* compare sum_digits/denom with best_num/best_den
           update only on strict improvement so first base wins ties */
        if (best_b == 0 || sum_digits * best_den > best_num * denom)
        {
            best_b = b;
            best_num = sum_digits;
            best_den = denom;
        }
    }

    cache[n] = best_b;                     /* memoize result */
    return (seq_t)best_b;
}
