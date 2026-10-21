/*
 * A279081  Number of divisors of the n-th tetrahedral number.
 * https://oeis.org/A279081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n*(n+1)*(n+2)/6 (the n‑th
 *            tetrahedral number)
 * Range:     u8 n <= 12, u16 n <= 1001, u32 n <= 2195, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not defined for n < 1, return 0 safely */

    seq_t N  = (seq_t)n;
    seq_t T  = N * (N + 1) * (N + 2) / 6;   /* exact division, product of 3 consecutive integers is divisible by 6 */

    return divisor_count_u64((uint64_t)T);  /* τ(T) */
}
