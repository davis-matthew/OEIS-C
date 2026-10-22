/*
 * A398392  a(n) is the number of inequivalent placements of 4 nonattacking
 *          rooks on an n X n board, up to rotations.
 * https://oeis.org/A398392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(24*(binomial(n,4))^2 + 2*k*(k-1) +
 *            2*(k^2)*(k-1)^2), where k = floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 29, u64 n <= 454, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* c = binomial(n,4) */
    seq_t c = binomial(n, 4);

    /* 6 * c^2  (i.e. 24*c^2 / 4) */
    seq_t result = 6 * c * c;

    idx_t k = n / 2;                 /* floor(n/2) */
    if (k >= 2) {
        seq_t ks  = (seq_t)k;
        seq_t k1  = (seq_t)(k - 1);

        /* (k*(k-1)) / 2 */
        result += (ks * k1) / 2;

        /* (k^2 * (k-1)^2) / 2 */
        result += (ks * ks * k1 * k1) / 2;
    }

    return result;
}
