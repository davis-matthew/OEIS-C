/*
 * A288872  Denominators for generalized Bernoulli numbers B[5,j](n), for
 *          j=1..4, n >= 0.
 * https://oeis.org/A288872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = product of all primes p such that p-1 | n, with p ≠ 5;
 *            For n = 0 return 1; for n = 1 return 2; for odd n > 1 return 1
 *            (which follows from the previous rule because p = 2 would be
 *            omitted)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; (seq_t)i * (seq_t)i <= (seq_t)x; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Special cases handled directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* For odd n > 1 the denominator is 1. */
    if (n & 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    /* Iterate over all positive divisors d of n. */
    for (idx_t d = 1; (seq_t)d * (seq_t)d <= (seq_t)n; ++d) {
        if (n % d == 0) {
            /* First divisor d */
            idx_t p = d + 1;
            if (p != 5 && is_prime_idx(p))
                result *= (seq_t)p;

            /* Complementary divisor n/d, avoid double‑counting the square root. */
            idx_t const d2 = n / d;
            if (d2 != d) {
                idx_t p2 = d2 + 1;
                if (p2 != 5 && is_prime_idx(p2))
                    result *= (seq_t)p2;
            }
        }
    }
    return result;
}
