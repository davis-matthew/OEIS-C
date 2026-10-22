/*
 * A374148  Integer part of (2^(n - 1) - 1)*sqrt(3) + 1.
 * https://oeis.org/A374148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sqrt(3)*(2^(n-1)-1) + 1 )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   floor_sqrt, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Compute floor(sqrt(x)) for a 128‑bit unsigned integer in a portable way */
static inline uint64_t floor_sqrt_u128(unsigned __int128 v)
{
    uint64_t lo = 0, hi = (uint64_t)-1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* 64‑bit (small) implementation – safe for values that fit in 64 bits */
static seq_t a_small(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    /* 2^(n-1) as a 128‑bit value – safe until n-1 < 128 */
    unsigned __int128 pow2 = (unsigned __int128)1 << (n - 1);
    unsigned __int128 x   = pow2 - 1;
    unsigned __int128 y   = x * x * 3;          /* y = 3*x^2 */
    uint64_t z = floor_sqrt_u128(y);           /* floor(sqrt(y)) */
    return (seq_t)(z + 1);
}

/* Arbitrary‑precision implementation for 128‑bit and larger seq_t */
static seq_t a_big(idx_t n)
{
    if (n == 0) return 0;
    seq_t power = seq_pow((seq_t)2, n - 1);      /* 2^(n-1)   */
    seq_t x = power - 1;                        /* 2^(n-1)-1 */
    seq_t y = seq_mul(x, x);                    /* x^2       */
    y = seq_mul(y, (seq_t)3);                   /* 3*x^2     */
    seq_t z = floor_sqrt(y);                    /* floor(sqrt(y)) */
    return z + 1;
}

/* Public entry point – dispatches to the appropriate implementation */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* 8‑byte seq_t has no support for numbers beyond 2^64-1 */
    if (sizeof(seq_t) <= 8) {
        /* For 64‑bit seq_t, we use the safe 128‑bit intermediate */
        return a_small(n);
    } else {
        /* For larger seq_t (128‑bit or arbitrary‑precision) use big integer ops */
        return a_big(n);
    }
}
