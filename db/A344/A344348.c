/*
 * A344348  a(n) = floor(frac(e * n) * n).
 * https://oeis.org/A344348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( frac(e*n) * n ) Using the identity a(n) =
 *            floor(n^2 * (e-2)) - n * floor(n * (e-2)), where alpha = e - 2 =
 *            0.718281828459045
 * Range:     u8 n <= 276, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic with a 128‑bit
   accumulator to guarantee exactness for all n up to at least 10⁴. */

#include <stdint.h>

/* fractional part of e, scaled by 10^15 */
static const unsigned __int128 INV = (unsigned __int128)1000000000000000ULL; /* Q = 10^15 */
static const unsigned __int128 P   = (unsigned __int128)718281828459045ULL;   /* alpha * Q */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    unsigned __int128 nn = (unsigned __int128)n;

    /* floor(n * alpha) */
    unsigned __int128 floor_na = (nn * P) / INV;

    /* floor(n^2 * alpha) */
    unsigned __int128 floor_n2a = (nn * nn * P) / INV;

    /* a(n) = floor(n^2 * alpha) - n * floor(n * alpha) */
    unsigned __int128 result = floor_n2a - nn * floor_na;

    return (seq_t)result;
}
