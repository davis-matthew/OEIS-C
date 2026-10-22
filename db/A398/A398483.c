/*
 * A398483  Triangle read by rows: T(n, k) = n^k * (n - 1)^(n - k).
 * https://oeis.org/A398483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r(r+1)/2 <= n < (r+1)(r+2)/2, T(r,k) = r^k *
 *            (r-1)^(r-k). For r=0 the value is 1
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 56, u64 n <= 151,
 *            u128 n <= 397, big any n
 * Helpers:   pow_seq_by_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_by_exp.h"

seq_t a(idx_t n)
{
    /* Find row r such that first index cum = r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + r + 1 <= n) {
        ++r;
        cum += r;
    }
    /* Position k within row r */
    idx_t k = n - cum;

    /* Special case r==0 gives value 1. */
    if (r == 0) return (seq_t)1;

    seq_t part1 = pow_seq_by_exp((seq_t)r, k);
    seq_t part2 = pow_seq_by_exp((seq_t)r - 1, r - k);
    return part1 * part2;
}
