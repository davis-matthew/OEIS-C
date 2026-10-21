/*
 * A256097  Numerators of a rational guess r(n) for the input for Newton's
 *          algorithm to find sqrt(n).
 * https://oeis.org/A256097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of ( floor(sqrt(n))^2 + n ) / (
 *            2*floor(sqrt(n)) ) reduced to lowest terms
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Integer floor sqrt for unsigned 64‑bit integers by binary search. */
static idx_t isqrt_u64(idx_t value)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;   /* 2^32 > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= value)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Euclidean algorithm for 128‑bit integers. */
static __uint128_t gcd128(__uint128_t a, __uint128_t b)
{
    while (b != 0) {
        __uint128_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the numerator of the rational R(n) used as the initial guess for
   Newton's method on sqrt(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* not part of the sequence,
                                                   but harmless */

    idx_t f = isqrt_u64(n);                    /* floor(sqrt(n)) */

    __uint128_t fn = f;
    __uint128_t num = fn * fn + (__uint128_t)n; /* f^2 + n */
    __uint128_t den = fn * 2;                  /* 2*f */

    __uint128_t g   = gcd128(num, den);        /* reduce the fraction */
    return (seq_t)(num / g);                   /* numerator after reduction */
}
