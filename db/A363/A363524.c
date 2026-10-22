/*
 * A363524  a(n) = 0 if 4 divides n + 1, otherwise (-1)^floor((n + 1) / 4) *
 *          2^floor(n / 2).
 * https://oeis.org/A363524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if 4 | (n+1); otherwise a(n) = (-1)^⌊(n+1)/4⌋ * 2^⌊n/2⌋
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Check for the zero terms: when n+1 is a multiple of 4. */
    if (((n + 1) & 3) == 0)
        return (seq_t)0;

    /* Determine the exponent of 2: floor(n/2). */
    idx_t exp = n >> 1;          /* equivalent to n / 2 */

    /* Determine the sign: (-1)^floor((n+1)/4). */
    seq_t sign = (( (n + 1) / 4 ) & 1) ? (seq_t)-1 : (seq_t)1;

    /* Compute 2^exp and apply the sign. */
    return sign * pow2(exp);
}
