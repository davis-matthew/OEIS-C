/*
 * A367968  Number of 6-cycles in the n-Dorogovtsev-Goltsev-Mendes graph.
 * https://oeis.org/A367968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (65*3^n - 84*n - 6*n^2 - 40*n^3 - 65)/8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t sn    = (seq_t)n;
    seq_t n2    = sn * sn;
    seq_t n3    = n2 * sn;

    seq_t term1 = (seq_t)65 * pow3;
    seq_t term2 = (seq_t)84  * sn;
    seq_t term3 = (seq_t)6   * n2;
    seq_t term4 = (seq_t)40  * n3;

    seq_t num = term1 - term2 - term3 - term4 - (seq_t)65;
    seq_t res = num / (seq_t)8;
    return res;
}
