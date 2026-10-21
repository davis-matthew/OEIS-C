/*
 * A253317  Indices in A261283 where records occur.
 * https://oeis.org/A253317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-A053644(n-1)) + 2^(A053644(n-1)-1), a(1)=0 where
 *            A053644(k) = largest power of two ≤ k
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Return floor(log2(x)) for x >= 1 (x is idx_t). */
static inline idx_t floor_log2_ull(idx_t x)
{
    return 63 - __builtin_clzll(x);   /* GCC/Clang builtin; x != 0 */
}

seq_t a(idx_t n)
{
    if (n == 0)                         /* a(0) is undefined: return 0 */
        return (seq_t)0;

    /* Resize cache if we need more entries. */
    if ((idx_t)n > (idx_t)cache_cap) {
        idx_t new_cap = n + 1;          /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                       /* out of memory – abort safely */
            return (seq_t)0;
        cache = tmp;

        /* Compute new entries using the recurrence. */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 0;          /* a(1) = 0 */
            } else {
                idx_t x = i - 1;       /* argument of A053644 */
                idx_t logx = floor_log2_ull(x);   /* floor(log2(x)) */
                idx_t pk  = (idx_t)1 << logx;     /* 2^logx = largest power of two <= x */
                seq_t add = pow2(pk - 1);          /* 2^(pk-1) */
                cache[i]  = cache[i - pk] + add;   /* recurrence */
            }
        }
        cache_cap = n;
    }

    return cache[n];
}
