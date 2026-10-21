/*
 * A290054  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of (Sum_{j>=0} x^(j^3))^k.
 * https://oeis.org/A290054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient obtained by reading the square array *
 *            A(n,k) (number of ways to write n as a sum of k nonnegative
 *            cubes) * along its antidiagonals. For an index i the
 *            corresponding antidiagonal m * and position pos are determined
 *            by * * m = smallest integer with (m+1)(m+2)/2 > i * pos =...
 * Range:     u8 n <= 123, u16 n <= 384, u32 n <= 1338, u64 n <= 4778,
 *            u128 n <= 9869, big any n
 * Helpers:   index_to_params, ways_cube_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/index_to_params.h"
#include "../utils/ways_cube_sum.h"

seq_t a(idx_t n)
{
    idx_t nn, kk;
    index_to_params(n, &nn, &kk);
    return ways_cube_sum(nn, kk);
}
