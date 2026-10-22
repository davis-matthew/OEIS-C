/*
 * A329445  Dirichlet inverse of A328745.
 * https://oeis.org/A329445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Product_{p^e || n} (-1)^e * binomial(p,e) with
 *            binomial(p,e)=0 if e>p. OEIS A329445 (Dirichlet inverse of
 *            A328745)
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   binomial_prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_prime_exp.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1)=1 */
    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= binomial_prime_exp(p, exp);
            if (result == 0) return 0;   /* early exit on zero */
        }
    }
    if (tmp > 1) {               /* remaining prime factor with exp = 1 */
        result *= binomial_prime_exp(tmp, 1);
    }
    return result;
}
