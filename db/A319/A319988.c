/*
 * A319988  a(n) = 1 if n is divisible by the square of its largest prime
 *          factor, 0 otherwise.
 * https://oeis.org/A319988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is divisible by the square of its largest prime
 *            factor, 0 otherwise
 * Range:     u8 n <= 9698, u16 n <= 9698, u32 n <= 9698, u64 n <= 9698,
 *            u128 n <= 9698, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    uint64_t val = (uint64_t)n;
    uint64_t p = largest_prime_divisor(val);
    if (p <= 1)
        return 0;
    uint64_t tmp = val / p;
    return (tmp % p == 0) ? 1 : 0;
}
