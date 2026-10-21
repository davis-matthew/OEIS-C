/*
 * A289804  p-INVERT of the even bisection (A001519) of the Fibonacci numbers,
 *          where p(S) = 1 - S - S^2.
 * https://oeis.org/A289804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 9*a(n-3) + a(n-4) with a(0) = 1,
 *            a(1) = 3, a(2) = 9, a(3) = 29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

/* compute nth term of sequence A289804 */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap  = 0;     /* capacity of cache array */
    static size_t size = 0;     /* number of already computed elements */

    /* ensure enough storage for up to index n */
    if ((size_t)n + 1 > cap)
        ensure_capacity((size_t)n + 1, &cap, (void**)&cache, sizeof(seq_t));

    /* initialize first four values if not done yet */
    if (size < 4) {
        /* ensure capacity for the first four */
        if (cap < 4)
            ensure_capacity(4, &cap, (void**)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)29;
        size = 4;
    }

    /* compute values up to n lazily */
    for (size_t i = size; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        cache[i] = 7*a1 - 15*a2 + 9*a3 + a4;
    }
    size = (size_t)n + 1;

    return cache[(size_t)n];
}
