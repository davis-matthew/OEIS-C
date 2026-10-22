/*
 * A372872  a(n) is the total number of runs of weak ascents over all
 *          flattened Catalan words of length n.
 * https://oeis.org/A372872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27 - 9*n + (5 + n)*3^n)/36
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow_n = big_pow((seq_t)3, (seq_t)n);

    /* (5 + n) * 3^n */
    seq_t term1 = ((seq_t)5 + (seq_t)n) * pow_n;

    /* 27 - 9*n */
    seq_t term2 = (seq_t)27 - (seq_t)9 * (seq_t)n;

    /* numerator = (5 + n)*3^n + 27 - 9*n */
    seq_t numerator = term1 + term2;

    /* divide by 36 */
    return numerator / (seq_t)36;
}
