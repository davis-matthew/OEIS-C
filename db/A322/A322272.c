/*
 * A322272  Smallest multiplication factors f, prime or 1, for all a (mod
 *          120), coprime to 120, so that b*f is a nonzero square mod 8, mod
 *          3, and mod 5.
 * https://oeis.org/A322272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_sq5, is_sq8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the smallest multiplier f (prime or 1) such that
 * b(n) * f is a nonzero quadratic residue modulo 8, 3 and 5.
 * Here b(n) is the n‑th integer coprime to 120 (OEIS A007775).
 * The function computes the result for any n ≥ 1, using the
 * periodicity of the residues modulo 120.  The implementation
 * is an O(1) operation after an inexpensive table lookup.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_sq5.h"
#include "../utils/is_sq8.h"

/* the 32 residues 1 ≤ x ≤ 120 that are coprime to 120, in order */
static const uint8_t residues[32] = {
    1, 7, 11, 13, 17, 19, 23, 29,
   31, 37, 41, 43, 47, 49, 53, 59,
   61, 67, 71, 73, 77, 79, 83, 89,
   91, 97,101,103,107,109,113,119
};

/* primes ≥7 that are coprime to 120 (excluding 2,3,5) */
static const uint8_t primes[27] = {
     7, 11, 13, 17, 19, 23, 29, 31,
    37, 41, 43, 47, 53, 59, 61,
    67, 71, 73, 79, 83, 89, 97,
   101,103,107,109,113
};

seq_t a(idx_t n)
{
    /* n is 1‑based; protect against n==0 */
    if (n == 0)
        return (seq_t)0;

    /* index into the period of length 32 */
    size_t idx = (size_t)((n - 1) % 32);
    uint8_t b = residues[idx];

    /* residues of b modulo the relevant moduli */
    uint8_t b8 = b & 7;          /* b mod 8 */
    uint8_t b3 = b % 3;          /* b mod 3 */
    uint8_t b5 = b % 5;          /* b mod 5 */

    /* test f = 1 */
    if (is_sq8(b8) && (b3 == 1) && is_sq5(b5))
        return (seq_t)1;

    /* test prime multipliers in ascending order */
    for (size_t i = 0; i < 27; ++i) {
        uint8_t f = primes[i];

        /* f is odd and not divisible by 3 or 5, so we can use simple mods */
        uint8_t f8 = f & 7;
        uint8_t f3 = f % 3;
        uint8_t f5 = f % 5;

        uint8_t prod8 = (b8 * f8) & 7;
        uint8_t prod3 = (b3 * f3) % 3;
        uint8_t prod5 = (b5 * f5) % 5;

        if (is_sq8(prod8) && (prod3 == 1) && is_sq5(prod5))
            return (seq_t)f;
    }

    /* fall back (should never happen for the defined range) */
    return (seq_t)1;
}
