/*
 * A290072  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 641", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 1 Let m = n-1. If n is odd: a(n) = (10^m - 1) /
 *            9 // decimal string of m ones If n is even: a(n) = 10^n + (10^m
 *            - 1) / 9 // '1' followed by '0' and m ones
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t p_nm1 = pow10(n - 1);
    seq_t t = (p_nm1 - (seq_t)1) / (seq_t)9;   // (10^(n-1)-1)/9

    if (n & 1)      /* odd n */
        return t;
    else            /* even n */
    {
        seq_t p_n = pow10(n);
        return p_n + t;
    }
}
