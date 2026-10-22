/*
 * A328640  Dirichlet g.f.: zeta(2*s) / (zeta(s) * zeta(s-3)).
 * https://oeis.org/A328640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(p^e) = (-1)^e*(p^3 + 1) (multiplicative)
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1229, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard          
                                      in case n==0 is passed            */
    uint64_t m = (uint64_t)n;
    seq_t res = 1;

    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        seq_t term = (seq_t)p * (seq_t)p * (seq_t)p + 1;
        res *= term;
        if (exp & 1) res = -res;    /* toggle sign for odd exponent       */
    }

    return res;
}
