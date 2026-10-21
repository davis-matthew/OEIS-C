/*
 * A245020  Number of ordered n-tuples of positive integers, whose minimum is
 *          0 and maximum is 5.
 * https://oeis.org/A245020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 2*5^n + 4^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 0; the function is defined for n >= 1 */
    if (n == 0) {
        return (seq_t)0;            /* undefined for offset 1, but safe */
    }

    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);

    /* Compute 6^n - 2*5^n + 4^n */
    seq_t result = pow6 - 2 * pow5 + pow4;
    return result;
}
