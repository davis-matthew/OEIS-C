/*
 * A396762  a(n) is the number of distinct triples of triangles chosen from
 *          n^2 unit triangles of a triangular grid up to rotation and
 *          reflection without having centers of all three triangles in a same
 *          symmetrical axis.
 * https://oeis.org/A396762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A396761(n) – binomial(n,3) The following implementation
 *            evaluates A396761(n) directly using the closed form derived from
 *            the group action on the n² unit triangles of the equilateral
 *            triangular grid
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 72, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert n to seq_t for calculations */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;                 /* n² */

    /* binomial(n²,3) = n²·(n²–1)·(n²–2)/6 */
    seq_t bin_n2_3 = n2 * (n2 - 1);
    bin_n2_3 *= (n2 - 2);
    bin_n2_3 /= 6;

    /* binomial(n,3) = n·(n–1)·(n–2)/6 */
    seq_t bin_n_3 = sn * (sn - 1);
    bin_n_3 *= (sn - 2);
    bin_n_3 /= 6;

    /* f = 0 if n is a multiple of 3, otherwise 1 */
    idx_t f = (n % 3 == 0) ? 0 : 1;

    /* numerator of the closed‑form expression */
    seq_t term1 = 6 * bin_n2_3;
    seq_t term2 = 4 * ((seq_t)n2 - (seq_t)f);
    seq_t term3 = 9 * sn * ((seq_t)n2 - sn);
    seq_t term4 = 18 * bin_n_3;

    seq_t numer = term1 + term2 + term3 - term4;

    return numer / 36;   /* division is exact */
}
