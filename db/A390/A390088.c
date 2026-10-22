/*
 * A390088  a(n) is the least integer base b > 1 in which the representation
 *          of n uses only prime digits; or 0 if no such base exists.
 * https://oeis.org/A390088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A390088: least base b > 1 such that the representation of n
   in base b uses only prime digits; returns 0 if no such base exists.
   Implements a brute‑force search over bases 2..n, then returns n+1
   if n itself is prime. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime.h"

static bool digits_prime(idx_t num, idx_t base)
{
    while (num > 0) {
        idx_t digit = num % base;
        if (digit <= 255) {
            if (!is_prime_small((seq_t)digit))
                return false;
        } else {
            if (!is_prime(digit))
                return false;
        }
        num /= base;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    for (idx_t base = 2; base <= n; ++base) {
        if (digits_prime(n, base))
            return (seq_t)base;
    }

    if (is_prime(n))
        return (seq_t)(n + 1);
    return (seq_t)0;
}
