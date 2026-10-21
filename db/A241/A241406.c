/*
 * A241406  Numbers n such that n^2 == -1 (mod 61).
 * https://oeis.org/A241406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -11*(-1)^n + 61*floor(n/2) Equivalent: a(n) =
 *            61*floor(n/2) + 11 if n is odd, else 61*floor(n/2) – 11 (This
 *            produces the numbers congruent to 11 or 50 modulo 61.)
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 61*floor(n/2) */
    seq_t res = (seq_t)(n >> 1) * 61;

    /* Add or subtract 11 depending on parity of n */
    if (n & 1)        /* odd n */
        res += 11;
    else              /* even n */
        res -= 11;

    return res;
}
