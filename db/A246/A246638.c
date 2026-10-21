/*
 * A246638  Sequence a(n) = 2 + 3*A001519(n+1) appearing in a certain four
 *          circle touching problem together with A246639.
 * https://oeis.org/A246638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) Initial values: a(0)=5,
 *            a(1)=8, a(2)=17
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   grow_seq, mul4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/mul4.h"

/* Helper: multiply by 4 with a wide intermediate to avoid overflow
   For C++ and arbitrary-precision types the normal * operation is safe.
   For plain C integral types we use __uint128_t to preserve the exact value. */
#ifndef __cplusplus

#else

#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* current allocation size */
    static size_t len = 0;   /* number of computed entries (next free index) */

    /* Ensure we have room for index n */
    if ((size_t)n + 1 > cap)
        grow_seq(&cache, &cap, (size_t)n + 1);

    /* Initialize base terms as needed */
    if (len == 0) {
        cache[0] = 5;        /* a(0) */
        len = 1;
    }
    if (len == 1 && n >= 1) {
        cache[1] = 8;        /* a(1) */
        len = 2;
    }
    if (len == 2 && n >= 2) {
        cache[2] = 17;       /* a(2) */
        len = 3;
    }

    /* Compute subsequent terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i)
        cache[i] = mul4(cache[i-1]) - mul4(cache[i-2]) + cache[i-3];

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
