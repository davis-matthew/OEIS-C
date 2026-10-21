/*
 * A283813  Poly-Bernoulli numbers B_n^(k) with k = -7.
 * https://oeis.org/A283813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Poly–Bernoulli numbers B_n^(−7) (OEIS A283813)
 * Formula: a(n) = 5040*8^n - 15120*7^n + 16800*6^n - 8400*5^n + 1806*4^n - 126*3^n + 2^n.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* store current powers of the bases 8,7,6,5,4,3,2 */
    seq_t p8 = 1, p7 = 1, p6 = 1, p5 = 1, p4 = 1, p3 = 1, p2 = 1;

    for (idx_t i = 0; i < n; ++i) {
        p8 *= (seq_t)8;
        p7 *= (seq_t)7;
        p6 *= (seq_t)6;
        p5 *= (seq_t)5;
        p4 *= (seq_t)4;
        p3 *= (seq_t)3;
        p2 *= (seq_t)2;
    }

    /* sum of the positive terms */
    seq_t pos = ((seq_t)5040) * p8  +  ((seq_t)16800) * p6
               + ((seq_t)1806) * p4  +  p2;

    /* sum of the negative terms */
    seq_t neg = ((seq_t)15120) * p7  +  ((seq_t)8400) * p5
               + ((seq_t)126)  * p3;

    return pos - neg;
}
