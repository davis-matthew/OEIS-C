/*
 * A344260  a(n) is the number of relations from an n-element set into a set
 *          of at most n elements.
 * https://oeis.org/A344260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n+n^2) - 1)/(2^n - 1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t pow_n = pow2(n);
    seq_t numerator = pow2(n + n * n) - (seq_t)1;
    seq_t denominator = pow_n - (seq_t)1;
    return numerator / denominator;
}
