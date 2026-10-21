/*
 * A294515  Denominators of partial sums of the reciprocals of the decagonal
 *          numbers.
 * https://oeis.org/A294515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of V(4,1;n) = denominator of \sum_{k=0}^{n}
 *            1/((k+1)*(4k+1)), written in lowest terms
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* numerator of current partial sum */
    seq_t den = 1;   /* denominator of current partial sum */

    for (idx_t k = 0; k <= n; ++k) {
        /* denominator of the k‑th term: (k+1)*(4k+1) */
        seq_t kk1 = (seq_t)(k + (idx_t)1);
        seq_t term_den = kk1 * ((seq_t)4 * (seq_t)k + (seq_t)1);

        /* add 1/term_den to the current fraction num/den */
        seq_t g1 = gcd(den, term_den);            /* gcd(den,term_den)   */
        seq_t den_over_g = den / g1;              /* den / gcd           */
        seq_t lcm_den = den_over_g * term_den;    /* lcm of den and term_den */

        seq_t n1 = (num * (term_den / g1)) + den_over_g; /* new numerator */

        seq_t g2 = gcd(n1, lcm_den);             /* reduce fraction */
        num = n1 / g2;
        den = lcm_den / g2;
    }

    return den;   /* denominator of V(4,1;n) in lowest terms */
}
