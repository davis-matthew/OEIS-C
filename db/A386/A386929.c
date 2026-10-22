/*
 * A386929  a(n) is the least base b in {2,...,10} such that the base-b
 *          expansion of n, when read as a decimal integer, is prime; a(n) = 0
 *          if no such base exists.
 * https://oeis.org/A386929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least b ∈ {2,…,10} such that the base‑b expansion of n,
 *            interpreted as a decimal integer, is prime; a(n) = 0 if none
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* to test primality of seq_t values */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Safety; per spec n ≥ 1 */

    /* Try every base from 2 to 10 inclusive */
    for (idx_t base = 2; base <= 10; ++base) {
        idx_t          tmp = n;
        uint8_t        digits[64];     /* enough for 64 bits of n */
        size_t         len  = 0;

        /* Decompose n into digits in the given base (least‑significant first) */
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % base);
            tmp /= base;
        }

        /* Build the decimal integer from the most‑significant digit */
        seq_t val = 0;
        for (size_t i = len; i > 0; --i) {
            val = val * 10 + digits[i - 1];
        }

        /* If this decimal integer is prime, we found the answer */
        if (is_prime_big(val)) {
            return (seq_t)base;
        }
    }
    return 0;   /* No base yields a prime */
}
