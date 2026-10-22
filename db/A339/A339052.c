/*
 * A339052  Odd bisection of the infinite Fibonacci word A096270.
 * https://oeis.org/A339052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+2)*φ) - floor((2n+1)*φ) - 1, where φ = (1+√5)/2
 * Range:     u8 n <= 1662, u16 n <= 9580, u32 n <= 9580, u64 n <= 9580,
 *            u128 n <= 9580, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned __int128,
   returns floor(√x).  Works for all arguments that fit in
   unsigned __int128, which covers all 64‑bit integers. */
static inline seq_t isqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = v;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

/* Compute floor(k * φ) for an unsigned integer k.
   Uses the identity floor(k*φ) = (k + floor(k*√5)) / 2.  */
static inline seq_t floor_phi(seq_t k)
{
    /* Compute floor(k*√5) = floor(√(5 * k²)) using 128‑bit arithmetic. */
    unsigned __int128 prod = (unsigned __int128)5 * (unsigned __int128)k * (unsigned __int128)k;
    seq_t sqrt5k = isqrt_uint128(prod);
    return (k + sqrt5k) / 2;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Work with 128‑bit to avoid overflow when n is large. */
    unsigned __int128 nn = n;
    seq_t k1 = (seq_t)(nn * 2 + 1);   /* 2n + 1 */
    seq_t k2 = (seq_t)(nn * 2 + 2);   /* 2n + 2 */

    seq_t v1 = floor_phi(k1);
    seq_t v2 = floor_phi(k2);

    return (seq_t)(v2 - v1 - 1);      /* result is always 0 or 1 */
}
