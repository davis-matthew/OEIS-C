/*
 * A356690  Product of the prime numbers that are between 10*n and 10*(n+1).
 * https://oeis.org/A356690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A356690
   Formula (OEIS notation): a(n) = ∏{ p prime | 10n < p ≤ 10(n+1) }
   (a(n) = 1 if no such prime)
*/

#include <stdbool.h>
#include <stdint.h>
/* Utility for prime testing on 64‑bit numbers */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Product of primes between 10*n and 10*(n+1) */
    seq_t prod = (seq_t)1;
    uint64_t low  = (uint64_t)n * 10ull + 1ull;   /* first number > 10n   */
    uint64_t high = (uint64_t)n * 10ull + 10ull;  /* last number ≤ 10(n+1)*/

    for (uint64_t p = low; p <= high; ++p) {
        if (is_prime_small(p)) {
            prod *= (seq_t)p;
        }
    }
    return prod;
}
