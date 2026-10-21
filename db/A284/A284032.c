/*
 * A284032  Poly-Bernoulli numbers B_n^(k) with k = -8.
 * https://oeis.org/A284032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40320*9^n - 141120*8^n + 191520*7^n - 126000*6^n * +
 *            40824*5^n - 5796*4^n + 254*3^n - 2^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute all powers once */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* Accumulate contributions */
    seq_t pos = 0;
    seq_t neg = 0;

    pos += (seq_t)40320 * pow9;
    neg += (seq_t)141120 * pow8;

    pos += (seq_t)191520 * pow7;
    neg += (seq_t)126000 * pow6;

    pos += (seq_t)40824 * pow5;
    neg += (seq_t)5796 * pow4;

    pos += (seq_t)254 * pow3;
    neg += pow2;                  /* coefficient is -1, not -2 */

    return pos - neg;
}
