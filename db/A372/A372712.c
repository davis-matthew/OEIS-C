/*
 * A372712  Numbers of minimal total dominating sets in the n-trapezohedral
 *          graph.
 * https://oeis.org/A372712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A001608(n)+n)^2 Recurrence (n ≥ 16): a(n) =
 *            4*a(n-1)-3*a(n-2)-4*a(n-3)-a(n-4)
 *            +14*a(n-5)-6*a(n-6)-5*a(n-7)-3*a(n-8)
 *            +7*a(n-9)-4*a(n-10)+3*a(n-11) -2*a(n-14)+a(n-15) Initial terms:
 *            a(1)=1, a(2)=16, a(3)=36, a(4)=36, a(5)=100, a(6)=121, a(7)=196,
 *            a(8)=324, a(9)=441, a(10)=729, a(11)=1089, a...
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 39, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initial 15 terms */
    static const seq_t init[15] = {
        1, 16, 36, 36, 100,
        121, 196, 324, 441, 729,
        1089, 1681, 2704, 4225, 6889
    };

    /* request within the defined offset */
    if (n <= 15)
        return init[n-1];

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_cap = 32;           /* initial capacity */
        cache = alloc_seq_array(cache_cap);
        /* copy known initial values */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        cache_len = 15;
    }

    /* extend cache capacity if needed */
    if (n > (idx_t)cache_len && n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        seq_t term;

        /* 4*a(i-1) */
        term = mul_small(cache[i-2], (unsigned)4);
        sum_pos = sum_pos + term;

        /* -3*a(i-2) */
        term = mul_small(cache[i-3], (unsigned)3);
        sum_neg = sum_neg + term;

        /* -4*a(i-3) */
        term = mul_small(cache[i-4], (unsigned)4);
        sum_neg = sum_neg + term;

        /* -1*a(i-4) */
        term = mul_small(cache[i-5], (unsigned)1);
        sum_neg = sum_neg + term;

        /* +14*a(i-5) */
        term = mul_small(cache[i-6], (unsigned)14);
        sum_pos = sum_pos + term;

        /* -6*a(i-6) */
        term = mul_small(cache[i-7], (unsigned)6);
        sum_neg = sum_neg + term;

        /* -5*a(i-7) */
        term = mul_small(cache[i-8], (unsigned)5);
        sum_neg = sum_neg + term;

        /* -3*a(i-8) */
        term = mul_small(cache[i-9], (unsigned)3);
        sum_neg = sum_neg + term;

        /* +7*a(i-9) */
        term = mul_small(cache[i-10], (unsigned)7);
        sum_pos = sum_pos + term;

        /* -4*a(i-10) */
        term = mul_small(cache[i-11], (unsigned)4);
        sum_neg = sum_neg + term;

        /* +3*a(i-11) */
        term = mul_small(cache[i-12], (unsigned)3);
        sum_pos = sum_pos + term;

        /* -2*a(i-14) */
        term = mul_small(cache[i-15], (unsigned)2);
        sum_neg = sum_neg + term;

        /* +1*a(i-15) */
        term = mul_small(cache[i-16], (unsigned)1);
        sum_pos = sum_pos + term;

        /* final value */
        cache[i-1] = sum_pos - sum_neg;
    }

    cache_len = n;
    return cache[n-1];
}
