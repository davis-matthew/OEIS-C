/*
 * A276678  Number of divisors of the n-th pentagonal number.
 * https://oeis.org/A276678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A000326(n)) = number of divisors of n*(3n-1)/2
 * Range:     u8 n <= 13, u16 n <= 261, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out of offset */
    /* pentagonal number P(n) = n*(3n-1)/2 */
    seq_t pent = (seq_t)n * ((((seq_t)3) * (seq_t)n) - 1) / 2;
    uint64_t pent_u64 = (uint64_t)pent;        /* fits for n up to at least 10^6 */
    return divisor_count_u64(pent_u64);
}
