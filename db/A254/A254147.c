/*
 * A254147  a(n) = 1*9^n + 2*8^n + 3*7^n + 4*6^n + 5*5^n + 6*4^n + 7*3^n +
 *          8*2^n + 9*1^n.
 * https://oeis.org/A254147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*9^n + 2*8^n + 3*7^n + 4*6^n + 5*5^n + 6*4^n + 7*3^n +
 *            8*2^n + 9*1^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

/* Computes the n-th term of OEIS sequence A254147.
   The sequence is defined for n >= 0. */
seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Sum i * (10-i)^n for i from 1 to 9. */
    for (idx_t i = 1; i <= 9; ++i)
    {
        seq_t base = (seq_t)(10 - i);          /* base = 10-i          */
        seq_t term = pow_int(base, n);         /* base^n            */
        result += (seq_t)i * term;             /* i * base^n */
    }

    return result;
}
