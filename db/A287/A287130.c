/*
 * A287130  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 245", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 0. For n > 2: if n is even : a(n) =
 *            11 * 10^(n-1) if n is odd, n ≤ 5 : a(n) = (10^n – 1) / 9 if n is
 *            odd, n > 5 : a(n) = (10^n – 10) / 90 Implements the conjectured
 *            rule 245 diagonal sequence
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)0;

    if ((n & 1) == 0) {                /* even n >= 4 */
        seq_t p10 = pow_uint(10, n - 1);/* 10^(n-1) */
        return (seq_t)11 * p10;
    }

    /* odd n >= 3 */
    seq_t p10 = pow_uint(10, n);       /* 10^n */
    if (n <= 5) {                      /* small odd values */
        return (p10 - (seq_t)1) / (seq_t)9;
    }
    /* large odd values */
    return (p10 - (seq_t)10) / (seq_t)90;
}
