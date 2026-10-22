/*
 * A307691  Clique covering and independence number of the n-Menger sponge
 *          graph.
 * https://oeis.org/A307691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more
 * Formula:   a(n) = 4^n * (4^n - (n%2==0 ? 3 : 1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; values below are irrelevant */

    /* Compute 4^n */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);

    /* Subtract 1 for odd n, 3 for even n */
    seq_t sub = (n & 1) ? (seq_t)1 : (seq_t)3;

    /* pow4 - sub, then multiply by pow4 */
    seq_t tmp = pow4 - sub;
    seq_t result = pow4 * tmp;

    return result;
}
