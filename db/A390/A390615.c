/*
 * A390615  a(n) = Kronecker symbol (28/n).
 * https://oeis.org/A390615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (28/n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   prime_sign
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_sign.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* zero if 2 or 7 divides n */
    if ((n & 1ULL) == 0 || (n % 7ULL) == 0)
        return 0;

    idx_t temp = n;
    seq_t prod = 1;

    /* factor odd primes and combine sign multiplicatively */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e ^= 1;            /* keep parity of the exponent */
            }
            if (e)                 /* odd exponent gives a factor of sign(p) */
                prod *= prime_sign(p);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
        prod *= prime_sign(temp);

    return prod;
}
