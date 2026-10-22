/*
 * A331360  Number of chiral pairs of colorings of the edges of a tesseract
 *          with n available colors.
 * https://oeis.org/A331360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (48*n^4 - 92*n^8 + 32*n^12 + 3*n^16 + 24*n^18 - 16*n^20 +
 *            n^32) / 384
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, value is arbitrary */
    seq_t N = (seq_t)n;

    seq_t n4  = big_pow(N, (seq_t)4);
    seq_t n8  = big_pow(N, (seq_t)8);
    seq_t n12 = big_pow(N, (seq_t)12);
    seq_t n16 = big_pow(N, (seq_t)16);
    seq_t n18 = big_pow(N, (seq_t)18);
    seq_t n20 = big_pow(N, (seq_t)20);
    seq_t n32 = big_pow(N, (seq_t)32);

    seq_t pos =      ((seq_t)48) * n4
                   + ((seq_t)32) * n12
                   + ((seq_t)3)  * n16
                   + ((seq_t)24) * n18
                   + n32;
    seq_t neg =      ((seq_t)92) * n8
                   + ((seq_t)16) * n20;

    seq_t diff = pos - neg;
    seq_t res  = diff / (seq_t)384;
    return res;
}
