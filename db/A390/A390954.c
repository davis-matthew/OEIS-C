/*
 * A390954  Dirichlet convolution of A390554 and A328254.
 * https://oeis.org/A390954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Π_{p^e‖n} f(p^e) where f(p) = -p, f(p^2) = p^3 - p^2 + p,
 *            f(p^3) = -p^3, f(p^e) = 0 for e > 3
 * Range:     i8 n <= 48, i16 n <= 1368, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   prime_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the multiplicative arithmetical function defined
   above.  This implementation factors n by trial division, suitable
   for the required range (up to ~10 000).  No external helper
   libraries are required. */
#include <stdint.h>
#include "../utils/prime_val.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 separately */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp > 0) {
        if (exp > 3) return (seq_t)0;
        result *= prime_val(2, exp);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            if (cnt > 3) return (seq_t)0;
            result *= prime_val(p, cnt);
        }
    }

    /* any remaining prime factor with exponent 1 */
    if (m > 1) {
        result *= prime_val(m, 1);
    }

    return result;
}
