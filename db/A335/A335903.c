/*
 * A335903  Column 1 in the matrix of A279212 (whose indexing starts at 0).
 * https://oeis.org/A335903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=6, a(3)=15, a(n) = (7*n + 9) * 2^(n-4) for n >= 4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)15;

    seq_t coeff = (seq_t)7 * (seq_t)n + (seq_t)9;
    seq_t pow_val = pow2(n - 4);          /* 2^(n-4) */
    return coeff * pow_val;
}
