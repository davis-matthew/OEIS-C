/*
 * A386341  Primes without {3, 6} as digits.
 * https://oeis.org/A386341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1970, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9967 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386341: the n‑th prime whose decimal digits contain no 3 or 6.
   Computed by brute‑force search: test successive integers, keep those
   that are prime and satisfy the digit condition. */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return true if x has no decimal digit 3 or 6. */
static bool has_no_36(idx_t x)
{
    while (x) {
        int d = x % 10;
        if (d == 3 || d == 6)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 2;

    while (1) {
        if (is_prime(candidate) && has_no_36(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* cast to seq_t is OK for any seq_t */
        }
        ++candidate;
    }
}
