/*
 * A334954  a(n) is 1 plus the number of divisors of n.
 * https://oeis.org/A334954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + A000005(n) (number of divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* divisor_count_u64 returns uint64_t; cast to seq_t before use */
    seq_t tau = (seq_t)divisor_count_u64(n);
    return tau + (seq_t)1;
}
