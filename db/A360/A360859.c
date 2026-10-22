/*
 * A360859  Triangle read by rows. T(n, k) = binomial(n, ceil(k/2)) *
 *          binomial(n, floor(k/2)).
 * https://oeis.org/A360859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, ceil(k/2)) * binomial(r, floor(k/2)) * where
 *            r is the unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2 * and k
 *            = n - r(r+1)/2. This defines the triangle read by rows
 * Range:     u8 n <= 25, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find the row r containing position n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* start index of row r */
    idx_t prev = r * (r + 1) / 2;
    /* position within the row */
    idx_t k = n - prev;

    /* floor and ceil of k/2 */
    idx_t floor_half = k / 2;
    idx_t ceil_half  = (k + 1) / 2;

    seq_t left  = binomial(r, ceil_half);
    seq_t right = binomial(r, floor_half);

    return left * right;
}
