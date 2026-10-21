/*
 * A290073  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 641", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2; for n > 1: a(n) = 2^(n+1) - 3 if n is even,
 *            a(n) = 2^(n+1) - 4 if n is odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) */
    if (n == 1) return (seq_t)2;  /* a(1) */

    seq_t val = pow2(n + 1);      /* 2^(n+1) */
    if ((n & 1ULL) == 0)          /* even n */
        return val - (seq_t)3;
    else                          /* odd n */
        return val - (seq_t)4;
}
