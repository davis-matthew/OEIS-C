/*
 * A349692  Dirichlet convolution of the gcd-sum function (A018804) with
 *          itself.
 * https://oeis.org/A349692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_contrib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet convolution of the gcd‑sum function (A018804) with itself.
   The function implements the multiplicative formula  
   a(p^e) = (e+1) * p^(e-2) * ((e+2)*(e+3)*p^2 - 2*e*(e+2)*p + e*(e-1)) / 6. */

#include <stdint.h>
#include "../utils/_prime_contrib.h"

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined, but harmless */
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 separately */
    int e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) result *= _prime_contrib((idx_t)2, e);

    /* trial division for odd primes up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= _prime_contrib(p, e);
        }
    }

    /* If remainder > 1, it is a prime factor > sqrt(n). */
    if (temp > 1) {
        result *= _prime_contrib(temp, 1);
    }

    return result;
}
