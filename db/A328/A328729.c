/*
 * A328729  Dirichlet g.f.: zeta(s) / (zeta(s-1) * zeta(2*s)).
 * https://oeis.org/A328729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Π_{p^e‖n} (2 - p - e) if e < 3 for all primes If any
 *            exponent e ≥ 3, a(n) = 0 (This implements the multiplicative
 *            definition given in the OEIS entry.)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;           /* a(1) = 1 */

    seq_t result = 1;
    idx_t tmp = n;
    size_t e = 0;

    /* factor out the prime 2 separately */
    if ((tmp & 1) == 0) {
        while ((tmp & 1) == 0) {
            ++e;
            tmp >>= 1;
            if (e >= 3) return 0;   /* exponent too large */
        }
        if (e > 0) {
            /* a(2^e) = 2 - 2 - e = -e  (for e = 1,2) */
            result = result * (seq_t)(-(seq_t)e);
        }
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
                if (e >= 3) return 0;   /* exponent too large */
            }
            if (e > 0) {
                /* a(p^e) = 2 - p - e */
                seq_t factor = (seq_t)2 - (seq_t)p - (seq_t)e;
                result *= factor;
            }
        }
    }

    /* any remaining prime factor has exponent 1 */
    if (tmp > 1) {
        seq_t factor = (seq_t)2 - (seq_t)tmp - (seq_t)1;   /* 1 - tmp */
        result *= factor;
    }

    return result;
}
