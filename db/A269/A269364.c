/*
 * A269364  Difference between the number of occurrences of prime gaps not
 *          divisible by 3, versus number of prime gaps that are multiples of
 *          3, up to n-th prime gap: a(n) = A269849(n) - A269850(n).
 * https://oeis.org/A269364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A269849(n) - A269850(n) (difference of prime gaps not
 *            divisible by 3 versus gaps that are multiples of 3 up to the
 *            n‑th gap)
 * Range:     u8 n <= 977, u16 n <= 8603, u32 n <= 8603, u64 n <= 8603,
 *            u128 n <= 8603, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t diff = 0;
    /* We need the first n+1 primes to compute n gaps.
       The helper get_prime(i) returns the i‑th prime with i ≥ 1. */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p1 = get_prime(i);
        uint64_t p2 = get_prime(i + 1);
        uint64_t gap = p2 - p1;
        if (gap % 3 == 0)
            diff -= 1;   /* gap divisible by 3 */
        else
            diff += 1;   /* gap not divisible by 3 */
    }
    return diff;
}
