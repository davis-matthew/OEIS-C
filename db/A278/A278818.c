/*
 * A278818  a(n) is the least k > n such that k + n is square.
 * https://oeis.org/A278818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(2n+1))^2 - n
 * Range:     u8 n <= 31, u16 n <= 7950, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute v = 2n + 1 */
    seq_t v = (seq_t)n * 2 + 1;

    /* floor square root of v */
    seq_t s = floor_sqrt(v);

    /* ceil square root: increase if not a perfect square */
    if (s * s < v) {
        s++;
    }

    /* square of the ceil root */
    seq_t sq = s * s;

    /* result = sq - n */
    return sq - n;
}
