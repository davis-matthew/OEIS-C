/*
 * A247842  a(n) = Sum_{k=2..n} 9^k.
 * https://oeis.org/A247842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=2}^{n} 9^k (a(1) = 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) == 0, and guard against accidental 0 */
        return (seq_t)0;

    seq_t sum  = (seq_t)81;  /* 9^2 – first term of the sum                */
    seq_t term = sum;        /* current power 9^k, starting with k = 2      */

    for (idx_t k = 3; k <= n; ++k) {
        /* term = 9^k ; multiply previous power by 9                */
        term = seq_mul(term, (seq_t)9);
        sum += term;        /* add to running total                         */
    }
    return sum;              /* final a(n)                                    */
}
