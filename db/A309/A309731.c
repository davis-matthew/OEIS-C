/*
 * A309731  Expansion of Sum_{k>=1} k * x^k/(1 - x^k)^3.
 * https://oeis.org/A309731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (d(n) + sigma(n))/2
 * Range:     u8 n <= 15, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard anyway */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);
    seq_t sigma = sigma_of(n);
    seq_t sn = (seq_t)n;
    return sn * (d + sigma) / 2;
}
