/*
 * A356636  Triangle read by rows. T(n, k) = binomial(n, k) * n!^2 /
 *          floor(n/2)!^2.
 * https://oeis.org/A356636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * Let r be the row such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, *
 *            and let c = n - r*(r+1)/2 be the column within that row. * Then
 *            a(n) = T(r,c) where T(r,c) = binomial(r,c) * r!² / floor(r/2)!².
 *            * The function uses the utility implementations of factorial()
 *            and * binomial() supplied by the build system
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r: the largest integer with r*(r+1)/2 ≤ n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index c within row r */
    idx_t c = n - r * (r + 1) / 2;

    /* Compute factorials */
    seq_t fac_r   = factorial(r);           /* r! */
    seq_t fac_half = factorial(r / 2);       /* floor(r/2)! */

    /* Compute the common factor A193282(r) = r!² / floor(r/2)!² */
    seq_t a193282 = fac_r * fac_r / (fac_half * fac_half);

    /* Finally compute T(r,c) = binomial(r,c) * A193282(r) */
    return binomial(r, c) * a193282;
}
