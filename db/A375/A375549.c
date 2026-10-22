/*
 * A375549  Series expansion of (x - 1)^3/(2*x - 1)^5.
 * https://oeis.org/A375549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)*(n + 6)*(n^2 + 15*n + 32)*2^(n-6)/3
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 20, u64 n <= 48, u128 n <= 108,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the polynomial numerator (n+1)(n+6)(n^2+15n+32) */
    seq_t t1 = (seq_t)(n + 1);
    seq_t t2 = (seq_t)(n + 6);
    seq_t t3 = ((seq_t)n * (seq_t)n) + ((seq_t)15 * (seq_t)n) + (seq_t)32;

    seq_t val = t1 * t2;
    val *= t3;

    /* Divide by 3 (always an integer). */
    val /= (seq_t)3;

    /* Multiply or divide by 2^(n-6) depending on the sign. */
    if (n >= 6) {
        val *= pow2((idx_t)(n - 6));
    } else {
        val /= pow2((idx_t)(6 - n));
    }

    return val;
}
