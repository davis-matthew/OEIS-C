/*
 * A242191  Expected value of the highest die when n six-sided dice are
 *          rolled, multiplied by 6^n.
 * https://oeis.org/A242191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(n+1) - 5^n - 4^n - 3^n - 2^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute 6^(n+1) */
    seq_t term6 = seq_pow((seq_t)6, n + 1);

    /* compute each power needed in 5^n ,4^n ,3^n ,2^n */
    seq_t term5 = seq_pow((seq_t)5, n);
    seq_t term4 = seq_pow((seq_t)4, n);
    seq_t term3 = seq_pow((seq_t)3, n);
    seq_t term2 = seq_pow((seq_t)2, n);

    /* assemble the result */
    seq_t result = term6 - term5 - term4 - term3 - term2 - (seq_t)1;
    return result;
}
