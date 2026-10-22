/*
 * A365771  a(n) = binomial(2*n+1, n)/(2*n+1) * binomial(3*n-1, n) for n >= 0.
 * https://oeis.org/A365771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+1, n)/(2*n+1) * binomial(3*n-1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the two binomial factors. */
    idx_t two_n_plus_one   = 2 * n + 1;
    idx_t three_n_minus_one = 3 * n - 1;

    seq_t part1 = binomial(two_n_plus_one, n);
    part1 = part1 / (seq_t)two_n_plus_one;   /* division is exact */

    seq_t part2 = binomial(three_n_minus_one, n);

    return part1 * part2;
}
