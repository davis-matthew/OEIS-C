/*
 * A302048  a(n) = 1 if n = p^2 for some prime p, otherwise 0. Characteristic
 *          function of squares of primes (A001248).
 * https://oeis.org/A302048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = p^2 for some prime p, otherwise 0
 * Range:     u8 n <= 9281, u16 n <= 9281, u32 n <= 9281, u64 n <= 9281,
 *            u128 n <= 9281, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* n = 0 or 1 cannot be a square of a prime */
    if (n <= 1) return 0;

    /* compute integer square root of n */
    idx_t r = isqrt_u64(n);

    /* check if n is a perfect square */
    if (r * r != n) return 0;

    /* check if the root is prime */
    return is_prime(r) ? 1 : 0;
}
