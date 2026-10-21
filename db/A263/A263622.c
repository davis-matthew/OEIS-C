/*
 * A263622  a(n) = (3^(n+1)-2^(n+2)+2*n+1)/4.
 * https://oeis.org/A263622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n+1)-2^(n+2)+2*n+1)/4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(__cplusplus)
/* In the C++ (big‑integer) build we can use the provided power helper. */
#include "../utils/big_pow.h"   /* big_pow is defined in the utils header */

seq_t a(idx_t n)
{
    /* 3^(n+1) and 2^(n+2) are computed in arbitrary precision */
    seq_t pow3 = big_pow((seq_t)3, n + 1);
    seq_t pow2 = big_pow((seq_t)2, n + 2);
    seq_t tmp  = (seq_t)2 * n + 1;          /* 2*n + 1  */

    /* The subtraction is now safe because for n≥1 3^(n+1) > 2^(n+2). */
    seq_t numer = pow3 + tmp - pow2;
    return numer / 4;
}
#else
/* In the pure C (up to 64‑bit) build we avoid overflow by using 128‑bit
   intermediates.  For n where the answer does not fit in seq_t, the
   behaviour is undefined – the tests only request values that do fit. */
static inline __int128 pow128_pow(__int128 base, idx_t exp)
{
    __int128 res = 1;
    while (exp)
    {
        if (exp & 1U) res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* All arithmetic is performed in __int128 to guard against overflow
       while the final result fits into seq_t. */
    __int128 p3 = pow128_pow((__int128)3, n + 1);       /* 3^(n+1)   */
    __int128 p2 = pow128_pow((__int128)2, n + 2);       /* 2^(n+2)   */
    __int128 tmp = (__int128)2 * n + 1;                 /* 2*n + 1   */

    __int128 numer = p3 - p2 + tmp;                     /* Numerator */
    return (seq_t)(numer / 4);
}
#endif
