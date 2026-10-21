/*
 * A282414  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 467", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1, a(1) = 2, a(n) = 0 for odd n > 1, a(n) =
 *            2^(n+1) – 1 for even n > 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided pow2 utility which produces a
   2^exp value in seq_t, handling arbitrary-precision integers when needed.
   The function works for all non‑negative n and for any seq_t width. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)                 return (seq_t)1;
    if (n == 1)                 return (seq_t)2;
    /* odd "n > 1" case */
    if (n & (idx_t)1)           return (seq_t)0;
    /* even case (n > 1) */
    seq_t v = pow2((idx_t)(n + 1));
    return v - (seq_t)1;
}
