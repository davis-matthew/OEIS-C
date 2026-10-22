/*
 * A339051  Even bisection of the infinite Fibonacci word A096270.
 * https://oeis.org/A339051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+1)r) - floor(2n r) - 1, r = golden ratio
 *            (A339051: even bisection of the infinite Fibonacci word A096270)
 * Range:     u8 n <= 9925, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Integer square root of a 128‑bit integer */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = ULLONG_MAX;      /* 2^64 - 1 is the maximum possible sqrt */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x)
            low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;                      /* floor(sqrt(x)) */
}

/* Floor(k * φ) where φ = (1+√5)/2, computed with integer arithmetic only */
static seq_t floor_phi(idx_t k)
{
    /* φ = (1 + √5)/2 => kφ = (k + k√5)/2.
       k√5 = floor(√(5k²)) because √5 * k = √(5k²) */
    unsigned __int128 kk = k;
    unsigned __int128 product = kk * kk * 5;   /* 5*k² */
    uint64_t sqrt_part = isqrt128(product);    /* floor(k√5) */
    return (seq_t)((kk + sqrt_part) / 2);
}

/* Sequence value a(n) */
seq_t a(idx_t n)
{
    idx_t k1 = n << 1;          /* 2n  */
    idx_t k2 = k1 + 1;           /* 2n+1 */
    seq_t term1 = floor_phi(k2); /* floor((2n+1)φ) */
    seq_t term0 = floor_phi(k1); /* floor(2nφ)   */
    return term1 - term0 - 1;
}
