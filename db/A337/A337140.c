/*
 * A337140  Numbers m = a + b with a and b positive integers whose product a*b
 *          = k^2 is a square.
 * https://oeis.org/A337140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that are either even or contain a prime factor ≡1 (mod 4). */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x (x≥1) has any prime factor ≡1 (mod 4).
   The function assumes x fits in uint64_t, which holds for all
   values tested by the sequence routine. */
static bool has_prime1mod4(uint64_t x)
{
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            if (p % 4 == 1) return true;
            do { x /= p; } while (x % p == 0);
        }
    }
    if (x > 1 && x % 4 == 1) return true;
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 2;          /* start from the first term a(1) = 2 */

    while (1) {
        /* a is either even, or odd but has a factor ≡1 (mod 4) */
        if (((seq_t)candidate & 1) == 0 ||
            has_prime1mod4((uint64_t)candidate))
        {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
