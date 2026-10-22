/*
 * A374878  Obverse convolution (3n+2)**(3n+2); see Comments.
 * https://oeis.org/A374878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+4)^(n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t base  = (seq_t)3 * n_seq + (seq_t)4;
    seq_t exp   = n_seq + (seq_t)1;
    return big_pow(base, exp);
}
