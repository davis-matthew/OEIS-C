/*
 * A294517  Denominators of the partial sums of the reciprocals of
 *          (k+1)*(4*k+3) = A033991(k+1), for k >= 0.
 * https://oeis.org/A294517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(∑_{k=0..n} 1/((k+1)*(4*k+3)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator */
    seq_t den = 1;   /* current denominator */

    for (idx_t k = 0; k <= n; ++k) {
        /* d = (k+1)*(4*k+3)  */
        seq_t k_seq   = (seq_t)k;
        seq_t kp1_seq = (seq_t)(k + 1);
        seq_t d1 = (seq_t)4 * k_seq + 3;
        seq_t d = kp1_seq * d1;

        /* Efficient addition: N/D + 1/d  */
        seq_t g = gcd(d, den);            /* gcd(d, den)      */
        seq_t d_div_g = d / g;            /* d / g            */
        seq_t den_div_g = den / g;        /* den / g          */

        /* New numerator and denominator before final reduction */
        seq_t num_new = num * d_div_g + den_div_g;
        seq_t den_new = den * d_div_g;

        /* Reduce the fraction */
        seq_t g2 = gcd(num_new, den_new);
        num = num_new / g2;
        den = den_new / g2;
    }

    return den;
}
