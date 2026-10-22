/*
 * A378069  a(n) = Sum_{k=0..n} binomial(n, floor(k/2 - 1/2)) * binomial(n,
 *          ceiling(k/2 - 1/2)).
 * https://oeis.org/A378069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{j=0}^{⌊(n-1)/2⌋} binomial(n, j)^2 +
 *            sum_{j=0}^{⌊(n-2)/2⌋} binomial(n, j) * binomial(n, j+1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* First part: squared binomials for odd k */
    idx_t limit1 = (n - 1) / 2;
    for (idx_t j = 0; j <= limit1; ++j) {
        seq_t b = binomial(n, j);
        sum += b * b;
    }

    /* Second part: cross products for even k */
    if (n >= 2) {
        idx_t limit2 = (n - 2) / 2;
        for (idx_t j = 0; j <= limit2; ++j) {
            seq_t b1 = binomial(n, j);
            seq_t b2 = binomial(n, j + 1);
            sum += b1 * b2;
        }
    }

    return sum;
}
