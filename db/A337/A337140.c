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
 * Helpers:   has_prime1mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that are either even or contain a prime factor ≡1 (mod 4). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_prime1mod4.h"

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
