/*
 * A397238  Dirichlet g.f.: Product_{p prime} (1 + 1/p^s + 1/p^(2*s-1)).
 * https://oeis.org/A397238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A397238
 *  Formula:  a(n) = product over prime powers in n
 *  Let n = Π p_i^e_i.
 *  a(p)     = 1
 *  a(p^2)   = p
 *  a(p^e)   = 0, for e >= 3
 *  Thus a(n) = 0 if any exponent >= 3; otherwise
 *  a(n) = Π p_i   (product of primes whose exponent is 2).
 *  (For n = 1 the empty product equals 1.)
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle n = 1 or small n directly */
    if (n < 2) {
        return (seq_t)1;
    }

    /* Use 64‑bit arithmetic for the factorisation */
    uint64_t x = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
                if (exp >= 3) {
                    /* Any exponent >= 3 annihilates the product */
                    return (seq_t)0;
                }
            }
            if (exp == 2) {
                result *= (seq_t)p;
            }
        }
    }

    /* Remaining factor, if any, is a prime with exponent 1 */
    /* No contribution to the product. */

    return result;
}
