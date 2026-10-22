/*
 * A329826  Beatty sequence for (5+sqrt(17))/4.
 * https://oeis.org/A329826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(5 + sqrt(17))/4)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* offset 1, n must be >= 1; safety for 0 is trivial */
    if (n == 0)
        return (seq_t)0;

    seq_t n_val = (seq_t)n;

    /* compute floor(n * sqrt(17)) as floor(sqrt(17 * n^2)) */
    seq_t prod = n_val * n_val;      /* n^2 */
    prod = prod * (seq_t)17;         /* 17 * n^2 */
    seq_t m = floor_sqrt(prod);      /* floor(n * sqrt(17)) */

    /* a(n) = floor((5*n + m) / 4) */
    seq_t result = ((seq_t)5 * n_val + m) / (seq_t)4;
    return result;
}
