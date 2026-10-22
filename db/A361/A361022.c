/*
 * A361022  a(n) = 1 if d(n) divides d(n+1), otherwise 0, where d(n) is number
 *          of positive divisors of n.
 * https://oeis.org/A361022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if d(n) divides d(n+1), where d(k) is the number of *
 *            positive divisors of k; otherwise a(n) = 0
 * Range:     u8 n <= 9880, u16 n <= 9880, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    if (n == 0)
        return (seq_t)0;

    /* Count divisors of n and n+1. */
    uint64_t d_n     = divisor_count_u64(n);
    uint64_t d_next  = divisor_count_u64(n + 1);

    /* Return 1 if d(n) divides d(n+1), else 0. */
    return (d_next % d_n == 0) ? (seq_t)1 : (seq_t)0;
}
