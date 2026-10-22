/*
 * A371911  Zeroless analog of tribonacci numbers.
 * https://oeis.org/A371911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Zr(a(n-1) + a(n-2) + a(n-3)); a(0)=a(1)=a(2)=1 * Zr(k)
 *            removes all zero digits from the decimal representation of k
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   zero_remove
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/zero_remove.h"

/* Zeroless tribonacci sequence. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0; /* allocated capacity                            */
    static idx_t len   = 0; /* number of values currently stored (max i seen) */

    /* Initial allocation. */
    if (cap == 0) {
#ifdef __cplusplus
        cache = new seq_t[4];
#else
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
#endif
        cap = 4;
        len = 0;
    }

    /* Ensure base values a(0)=a(1)=a(2)=1 are present. */
    for (idx_t i = 0; i < 3 && i < cap; ++i) {
        if (i >= len) {
            cache[i] = (seq_t)1;
            ++len;
        }
    }

    if (n < 3)
        return (seq_t)1;

    /* Grow the cache if necessary. */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (idx_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
#endif
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 3];
            cache[i] = zero_remove(sum);
        }
    }

    if (n + 1 > len)
        len = n + 1;

    return cache[n];
}
