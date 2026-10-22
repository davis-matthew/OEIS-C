/*
 * A392315  Number of vertex cuts in the n-necklace graph.
 * https://oeis.org/A392315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n - 3^n - 3*(5*3^n - 7)*n/2 - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit integer type for intermediate computations when available.   */
#if defined(__INT128__) || defined(__SIZEOF_INT128__)
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;
#else
/* If 128‑bit is unavailable, fall back to the largest built‑in type.  */
typedef unsigned long long uint128_t;  /* may overflow for very large n */
#endif

/* Compute b^e as an unsigned 128‑bit integer.  e is 0 or more.        */
static uint128_t pow128(unsigned int b, idx_t e)
{
    uint128_t res = 1;
    uint128_t base = b;
    while (e > 0) {
        if (e & 1) res *= base;
        base *= base;
        e >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Edge case: the sequence starts at n = 1.  For safety, return 0 for n < 1. */
    if (n == 0) return 0;

    /* Compute the powers of 3 and 16 using 128‑bit intermediates.   */
    uint128_t pow3 = pow128(3, n);
    uint128_t pow16 = pow128(16, n);

    /* part = 3*(5*3^n - 7)*n/2  (all computations in 128‑bit). */
    uint128_t part = ((5 * pow3 - 7) * 3 * (uint128_t)n) / 2;

    /* f = 16^n - 3^n - part - 1.                                     */
    uint128_t result = pow16 - pow3 - part - 1;

    /* Cast to seq_t.  For C++ builds where seq_t is an arbitrary‑precision
       type, an implicit conversion constructor will be invoked.      */
    return (seq_t)result;
}
