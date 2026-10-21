/*
 * A278617  Number of distinct odd primes less than or equal to 2n-3 that
 *          appear as a part in the partitions of 2n into two parts.
 * https://oeis.org/A278617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n ≤ 2; a(n) = π(2n−3) − 1 for n > 2 where π(x) is
 *            the prime-counting function
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)2)
        return (seq_t)0;
    idx_t lim = (idx_t)(2 * n - (idx_t)3);
    seq_t pi = compute_pi(lim);
    return pi - (seq_t)1;
}
