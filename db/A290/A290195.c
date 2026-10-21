/*
 * A290195  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 705", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 5, a(3) = 11, for even n ≥ 4 : a(n) =
 *            2^(n-1) – 1 for odd n ≥ 5 : a(n) = 3·2^(n-1) – 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    seq_t p = pow2(n - 1);           /* 2^(n-1) */
    if ((n & 1) == 0)                /* even n */
        return p - (seq_t)1;
    else                              /* odd n */
        return (p * (seq_t)3) - (seq_t)1;
}
