/*
 * A354923  a(n) = 1 if n is either a power of prime or 2*prime, otherwise 0.
 * https://oeis.org/A354923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is either a power of a prime or 2×prime, otherwise
 *            0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9898 (2026-11-01)
 * Helpers:   is_prime, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      // behaviour below offset is irrelevant
    if (n == 1) return (seq_t)1;      // OEIS definition: a(1) = 1

    /* Check if n is a prime power: it has exactly one distinct prime factor. */
    if (omega((uint64_t)n) == 1) return (seq_t)1;

    /* Check if n is twice a prime: n even and n/2 is prime. */
    if ((n & (idx_t)1) == 0) {
        idx_t half = n / 2;
        if (is_prime(half)) return (seq_t)1;
    }

    return (seq_t)0;
}
