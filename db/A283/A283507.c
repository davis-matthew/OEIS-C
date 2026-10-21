/*
 * A283507  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 641", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, for n > 0: if n is odd: a(n) = 2^(n-1) – 1 if n is
 *            even: a(n) = 3·2^(n-1) – 1 (derived from a(n) = (-2 + (-2)^n +
 *            2^(1+n)) / 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pw = pow2(n - 1);          /* 2^(n-1) */
    if ((n & 1ULL) != 0)            /* n odd */
        return pw - (seq_t)1;
    else                            /* n even */
        return pw * (seq_t)3 - (seq_t)1;
}
