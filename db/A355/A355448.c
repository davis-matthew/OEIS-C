/*
 * A355448  a(n) = 1 if the number of divisors of n^2 is coprime to 6,
 *          otherwise 0.
 * https://oeis.org/A355448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if all exponents of n in its prime factorisation
 *            satisfy e % 3 != 1, otherwise 0. Equivalently, every exponent e
 *            of n is NOT congruent to 1 modulo 3. This comes from the fact
 *            that d(n²) = ∏(2e+1) and d(n²) is coprime to 6 iff none of the
 *            factors (2e+1) is divisible by 3
 * Range:     u8 n <= 9701, u16 n <= 9701, u32 n <= 9701, u64 n <= 9701,
 *            u128 n <= 9701, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is implemented using simple trial division.  The
   result fits in the return type seq_t (either 0 or 1). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {  // undefined for 0, but guard for safety
        return (seq_t)0;
    }

    uint64_t x = (uint64_t)n;      // work in 64‑bit arithmetic
    /* check exponent of 2 */
    if (x % 2 == 0) {
        unsigned int exp = 0;
        while (x % 2 == 0) {
            x /= 2;
            ++exp;
        }
        if (exp % 3 == 1) {
            return (seq_t)0;
        }
    }

    /* check odd primes up to sqrt(x) */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            unsigned int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (exp % 3 == 1) {
                return (seq_t)0;
            }
        }
    }

    /* any remaining factor x is prime with exponent 1 */
    if (x > 1) {
        if (1 % 3 == 1) {          /* exp == 1 => 1 % 3 == 1 */
            return (seq_t)0;
        }
    }

    return (seq_t)1;
}
