/*
 * A257900  Expansion of 1/2 - (phi(-q)^2 + phi(-q^9)^2) / 4 in powers of q
 *          where phi() is a Ramanujan theta function.
 * https://oeis.org/A257900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative * a(2^e) = -1 (e>0) * a(3^e) = 2 if e
 *            even, 0 if e odd * a(p^e) = e+1 if p ≡ 1 (mod 4) * a(p^e) = 1 if
 *            p ≡ 3 (mod 4) and e even, * 0 if p ≡ 3 (mod 4) and e odd
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* smallest_prime_factor(n): returns the smallest prime factor of n;
 * if n is prime, returns n itself.  For n<2 returns n. */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n < 2ULL) return n;
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) return d;
    }
    return n;   /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    uint64_t x = (uint64_t)n;

    while (x > 1) {
        uint64_t p = smallest_prime_factor(x);
        uint64_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }

        seq_t factor;
        if (p == 2ULL) {
            factor = -1;
        } else if (p == 3ULL) {
            factor = (exp % 2 == 0) ? 2 : 0;
        } else if (p % 4ULL == 3ULL) {
            factor = (exp % 2 == 0) ? 1 : 0;
        } else { /* p % 4 == 1 */
            factor = (seq_t)(exp + 1);
        }

        result *= factor;
        if (result == 0) return 0;   /* early exit */
    }

    return result;
}
