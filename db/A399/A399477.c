/*
 * A399477  a(n) = 9*binomial(n,8) + 49*binomial(n,7) + 75*binomial(n,6) +
 *          35*binomial(n,5) + 3*binomial(n,4).
 * https://oeis.org/A399477
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*binomial(n,8) + 49*binomial(n,7) + 75*binomial(n,6) +
 *            35*binomial(n,5) + 3*binomial(n,4)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 44, u64 n <= 730,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;              /* offset guard – not used for valid n */
    seq_t t4 = binom(n, 4);
    seq_t t5 = binom(n, 5);
    seq_t t6 = binom(n, 6);
    seq_t t7 = binom(n, 7);
    seq_t t8 = binom(n, 8);
    return 3*t4 + 35*t5 + 75*t6 + 49*t7 + 9*t8;
}
