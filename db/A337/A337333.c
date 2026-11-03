/*
 * A337333  Number of pairs of odd divisors of n, (d1,d2), such that d1 <= d2.
 * https://oeis.org/A337333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (τ(odd_part(n)) * (τ(odd_part(n)) + 1)) / 2 where τ(m) is
 *            the number of divisors of m
 * Range:     u8 n <= 3464, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 9662 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* Strip all factors of 2 from n to get its odd part. */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    /* Number of odd divisors of n equals τ(odd). */
    seq_t odd_divisors = (seq_t)num_divisors(odd);

    /* Number of unordered pairs (d1,d2) with d1 <= d2. */
    return odd_divisors * (odd_divisors + 1) / 2;
}
