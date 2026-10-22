/*
 * A334641  a(n) is the total number of down steps between the 3rd and 4th up
 *          steps in all 2-Dyck paths of length 3*n.
 * https://oeis.org/A334641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Sum_{j=1..3} binomial(3*j+1, j)*binomial(3*(n-j), n-j)
 *            / /((3*j+1)*(n-j+1)) for n > 2 a(0) = a(1) = a(2) = 0 (see OEIS
 *            A334641)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* external linkage */
seq_t a(idx_t n)
{
    if (n <= 2U)
        return 0;

    seq_t sum = 0;
    for (idx_t j = 1; j <= 3; ++j) {
        idx_t nj = n - j;

        seq_t c1 = binomial(3 * j + 1, j);
        seq_t c2 = binomial(3 * nj, nj);

        seq_t prod = c1 * c2 * 2;
        seq_t denom = (seq_t)(3 * j + 1) * (seq_t)(nj + 1);

        sum += prod / denom;
    }
    return sum;
}
