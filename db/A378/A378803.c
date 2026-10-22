/*
 * A378803  a(n) = n^2 * binomial(4*n, n).
 * https://oeis.org/A378803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * binomial(4*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t comb = binomial(4 * n, n);
    seq_t m = (seq_t)n;
    return m * m * comb;
}
