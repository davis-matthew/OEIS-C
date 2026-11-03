/*
 * A241219  Number of ways to choose two points on a centered hexagonal grid
 *          of size n.
 * https://oeis.org/A241219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n^2 - 3*n + 1, 2) (OEIS A241219)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 176, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 836 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1; return 0 for n < 1 (offset guard) */
    if (n < 1)
        return 0;

    /* Compute the center hexagonal number */
    idx_t h = 3*n*n - 3*n + 1;

    /* a(n) is the binomial coefficient C(h,2) */
    return binomial(h, 2);
}
