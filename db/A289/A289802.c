/*
 * A289802  p-INVERT of the quarter-squares (A002620), where p(S) = 1 - S -
 *          S^2.
 * https://oeis.org/A289802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=15, a(3)=53, a(4)=185, a(5)=643, a(6)=2234,
 *            a(7)=7764;
 *            a(n)=5*a(n-1)-5*a(n-2)-4*a(n-3)+12*a(n-4)-5*a(n-5)-4*a(n-6)+4*a(n-7)-a(n-8)
 *            for n≥8
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                    /* size_t */
#include "../utils/grow_seq_array.h"   /* grow_seq_array() */

seq_t a(idx_t n)
{
    /* Cache previous terms for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache can hold the requested index (inclusive) */
    if (n >= cache_len) {
        grow_seq_array(&cache, &cache_len, (size_t)n + 1);
    }

    /* Initial 8 terms */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)4, (seq_t)15, (seq_t)53,
        (seq_t)185, (seq_t)643, (seq_t)2234, (seq_t)7764
    };

    /* Fill cache up to the current length if not already filled */
    size_t i;
    for (i = 0; i < 8 && i < cache_len; ++i) {
        cache[i] = init[i];
    }

    /* Compute missing terms using the recurrence */
    for (i = 8; i < cache_len; ++i) {
        /* pos = 5*a(n-1) + 12*a(n-4) + 4*a(n-7) */
        seq_t pos = (seq_t)5 * cache[i - 1]
                  + (seq_t)12 * cache[i - 4]
                  + (seq_t)4 * cache[i - 7];

        /* neg = 5*a(n-2) + 4*a(n-3) + 5*a(n-5) + 4*a(n-6) + a(n-8) */
        seq_t neg = (seq_t)5 * cache[i - 2]
                  + (seq_t)4 * cache[i - 3]
                  + (seq_t)5 * cache[i - 5]
                  + (seq_t)4 * cache[i - 6]
                  + cache[i - 8];

        /* a(n) = pos - neg; unsigned subtraction is safe as pos ≥ neg */
        cache[i] = pos - neg;
    }

    /* Return the requested term */
    return cache[(size_t)n];
}
