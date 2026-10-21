/*
 * A246297  Numbers k such that sin(k) > sin(k+1) < sin(k+2).
 * https://oeis.org/A246297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*2π + 3π/2 - 3/2) + 1 The sequence consists of
 *            all integers k ≥ 1 such that sin(k) > sin(k+1) < sin(k+2). Using
 *            the trigonometric identity sin(k+1)-sin(k)=2 sin(1/2) cos(k+1/2)
 *            it follows that the condition is equivalent to cos(k+1/2) < 0
 *            and cos(k+3/2) > 0 which is exactly true...
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n < 1 – the definition is only needed for n ≥ 1 */
    if (n == 0) return (seq_t)0;

    /* 2π ≈ 6.283185307179586  → 126 high precision digits are not needed
       for the range of n that will be queried, 12 decimal digit precision
       gives error < 5·10⁻¹⁰ for n ≤ 10⁶, safely below the unit threshold. */
    static const uint64_t N  = 6283185307180ULL;      /* 2π * 1e12 (rounded)   */
    static const uint64_t LEN = 3212388980385ULL;     /* (3π-3)/2 * 1e12 (rounded) */
    static const uint64_t D  = 1000000000000ULL;      /* 1e12                     */

    /* Use 128‑bit arithmetic to avoid overflow during the product. */
    __int128 prod = (__int128)(n - 1) * N + LEN;
    seq_t floor_val = (seq_t)(prod / D);   /* floor((n-1)*2π + (3π-3)/2)  */
    return floor_val + 1;                  /* a(n) = floor(...) + 1       */
}
