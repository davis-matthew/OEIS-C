/*
 * A398393  a(n) is the number of inequivalent placements of 4 nonattacking
 *          rooks on an n X n board, up to rotations and reflections.
 * https://oeis.org/A398393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(n,4)^2 + binomial(floor(n/2),2)^2 +
 *            (1/2)*(binomial(floor(n/2),2) + 5*binomial(n,4))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 32, u64 n <= 495, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t k = n / 2;

    /* binomial(n,4) and binomial(k,2) */
    seq_t cn4 = binomial(n, 4);
    seq_t ck2 = binomial(k, 2);

    /* 3 * C(n,4)^2 */
    seq_t term1 = 3 * cn4 * cn4;

    /* C(k,2)^2 */
    seq_t term2 = ck2 * ck2;

    /* (C(k,2) + 5*C(n,4)) / 2 */
    seq_t term3 = (ck2 + 5 * cn4) / 2;

    return term1 + term2 + term3;
}
