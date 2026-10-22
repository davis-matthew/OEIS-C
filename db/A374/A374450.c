/*
 * A374450  Number of edge covers of the fan graph F_{n,4}.
 * https://oeis.org/A374450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*15^n - 12*7^n + 9*3^n - 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow15 = big_pow((seq_t)15, (seq_t)n);
    seq_t pow7  = big_pow((seq_t)7,  (seq_t)n);
    seq_t pow3  = big_pow((seq_t)3,  (seq_t)n);

    seq_t result =
          (seq_t)8 * pow15
        - (seq_t)12 * pow7
        + (seq_t)9  * pow3
        - (seq_t)4;

    return result;
}
