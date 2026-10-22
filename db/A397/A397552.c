/*
 * A397552  a(n) = n! mod A000041(n), where A000041 is the partition function.
 * https://oeis.org/A397552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! mod p(n), where p(n) is the partition function
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   mul_seq_by_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_seq_by_uint.h"

seq_t a(idx_t n)
{
    /* Static cache for partition values p(0)...p(cache_max). */
    static seq_t *p_cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_max = 0;          /* highest index computed so far */

    /* Initial allocation. */
    if (p_cache == NULL)
    {
        cache_cap = 16;
        p_cache  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!p_cache) return 0;           /* allocation failure */
    }

    /* Grow the cache if the requested index is not yet allocated. */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        p_cache = tmp;
        cache_cap = new_cap;
    }

    /* Base partition values. */
    if (cache_max < 1)
    {
        p_cache[0] = (seq_t)1;
        p_cache[1] = (seq_t)1;
        cache_max  = 1;
    }

    /* Compute required partition numbers using the pentagonal-number theorem. */
    for (idx_t m = cache_max + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        for (idx_t k = 1;; ++k)
        {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            int   sign  = (k & 1U) ? 1 : -1;

            if (pent1 > m && pent2 > m) break;

            if (pent1 <= m)
            {
                if (sign > 0) sum += p_cache[m - pent1];
                else          sum -= p_cache[m - pent1];
            }
            if (pent2 <= m)
            {
                if (sign > 0) sum += p_cache[m - pent2];
                else          sum -= p_cache[m - pent2];
            }
        }
        p_cache[m] = sum;
    }
    cache_max = n;

    /* Compute n! modulo p(n). */
    seq_t mod = p_cache[n];
    if (mod == 0) return 0;                 /* should not happen */
    seq_t fact = (seq_t)1 % mod;
    for (idx_t i = 2; i <= n; ++i)
        fact = mul_seq_by_uint(fact, i, mod);

    return fact;
}
