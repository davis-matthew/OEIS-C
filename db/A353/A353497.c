/*
 * A353497  The smallest prime factor of n, reduced modulo 4, with a(1) = 1.
 * https://oeis.org/A353497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 1, otherwise the smallest prime factor of n
 *            reduced modulo 4
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9391 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    uint64_t spf = smallest_prime_factor((uint64_t)n);
    return (seq_t)(spf % 4);
}
