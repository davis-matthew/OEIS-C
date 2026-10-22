/*
 * A378921  Number of binary n X n matrices that do not possess both
 *          horizontal and vertical symmetry.
 * https://oeis.org/A378921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - 2^(ceil(n/2)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* total number of binary n×n matrices: 2^(n^2) */
    idx_t nsq = n * n;
    seq_t total = pow_uint(2ULL, nsq);

    /* subtract the matrices that are both horizontally and vertically symmetric.
       The number of independent cells equals ceil(n/2)^2. */
    idx_t half = (n + 1) >> 1;        /* ceil(n/2) */
    idx_t halfsq = half * half;
    seq_t sym = pow_uint(2ULL, halfsq);

    return total - sym;
}
