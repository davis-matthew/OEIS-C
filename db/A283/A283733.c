/*
 * A283733  a(n) = a(n-1) + 1 + floor(n*golden ratio), with a(0) = 1.
 * https://oeis.org/A283733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1 + floor(n*phi), a(0) = 1 * * Using the
 *            identity floor(n*phi) = (n + floor(sqrt(5*n*n))) / 2 * where phi
 *            = (1+sqrt(5))/2. * The computation is done with integer
 *            arithmetic only, using the supplied * floor_sqrt() helper.
 *            Intermediate products are performed in the idx_t * type (uin...
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 1;           /* a(0) */
    idx_t i;

    for (i = 1; i <= n; ++i) {
        seq_t k = (seq_t)i;
        idx_t tmp = 5 * i * i;                 /* 5 * k^2 computed in idx_t */
        seq_t sqrt_term = floor_sqrt((seq_t)tmp);
        seq_t floor_k_phi = (k + sqrt_term) / 2; /* integer division */

        sum += 1 + floor_k_phi;
    }

    return sum;
}
