/*
 * A339824  Even bisection of the infinite Fibonacci word A003849.
 * https://oeis.org/A339824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 - floor((2n+2)φ) + floor((2n+1)φ) where φ = (1+√5)/2.
 *            Using the convergent 987/610, which satisfies |φ - 987/610| <
 *            1/(610²), we can replace floor(kφ) by floor(k·987/610) for all k
 *            up to 2·10⁴ (and indeed for all k < 2⁶⁶)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   floor_phi_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation works for any seq_t and idx_t defined
   by the build environment.  It avoids floating point arithmetic by
   using 128‑bit integer operations only. */

#include <stdint.h>
#include "../utils/floor_phi_u128.h"

/* Sequence generator for A339824 (even bisection of Fibonacci word). */
seq_t a(idx_t n)
{
    /* Compute 2n+1 and 2n+2 as 128‑bit values to avoid overflow. */
    unsigned __int128 k1 = (unsigned __int128)n * 2 + 1;
    unsigned __int128 k2 = (unsigned __int128)n * 2 + 2;

    seq_t f1 = floor_phi_u128(k1);
    seq_t f2 = floor_phi_u128(k2);

    seq_t diff = f2 - f1;          /* diff is either 1 or 2 */
    return (diff == 1) ? 1 : 0;     /* 1 for diff=1, else 0           */
}
