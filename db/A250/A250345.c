/*
 * A250345  Numerator of the harmonic mean of the first n heptagonal numbers.
 * https://oeis.org/A250345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of harmonic mean of the first n heptagonal
 *            numbers
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t p = 0;   /* numerator of the sum of reciprocals */
    seq_t q = 1;   /* denominator of the sum of reciprocals */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t hi = (ii * (((seq_t)5 * ii) - (seq_t)3)) / 2;   /* heptagonal number */

        seq_t new_p = p * hi + q;
        seq_t new_q = q * hi;
        seq_t g = gcd(new_p, new_q);

        p = new_p / g;
        q = new_q / g;
    }

    seq_t num = (seq_t)n * q;
    seq_t g = gcd(num, p);
    return num / g;
}
