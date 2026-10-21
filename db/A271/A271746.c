/*
 * A271746  Number of set partitions of [n] such that 7 is the largest element
 *          of the last block.
 * https://oeis.org/A271746
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32 + 121*2^(n-7) + 155*3^(n-7) + 5*4^(n-5) + 16*5^(n-7) +
 *            6^(n-7) for n>7, a(7)=406
 * Range:     u8 none, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 56,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 7) return 0;
    if (n == 7) return 406;

    int e1 = (int)(n - 7);
    int e2 = (int)(n - 5);

    seq_t t = 32;

    seq_t p2 = pow_seq((seq_t)2, e1);
    seq_t p3 = pow_seq((seq_t)3, e1);
    seq_t p4 = pow_seq((seq_t)4, e2);
    seq_t p5 = pow_seq((seq_t)5, e1);
    seq_t p6 = pow_seq((seq_t)6, e1);

    t += (seq_t)121 * p2;
    t += (seq_t)155 * p3;
    t += (seq_t)5   * p4;
    t += (seq_t)16  * p5;
    t += p6;

    return t;
}
