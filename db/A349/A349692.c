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
 * Validated: standard, pass, 71 terms through n = 9176 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet convolution of the gcd‑sum function (A018804) with itself.
   The function implements the multiplicative formula  
   a(p^e) = (e+1) * p^(e-2) * ((e+2)*(e+3)*p^2 - 2*e*(e+2)*p + e*(e-1)) / 6. */

#include <stdint.h>

/* Helper that computes the contribution of a prime power p^e. */
static seq_t _prime_contrib(idx_t p, int e)
{
    /* Inner factor: (e+2)*(e+3)*p^2 - 2*e*(e+2)*p + e*(e-1) */
    seq_t inner = (seq_t)(e + 2) * (e + 3) * p * p
                - (seq_t)2 * e * (e + 2) * p
                + (seq_t)e * (e - 1);

    /* Numerator: (e+1) * inner */
    seq_t num = (seq_t)(e + 1) * inner;

    if (e == 1) {
        /* p^(e-2) = 1/p, so divide by 6*p */
        return num / ((seq_t)6 * p);
    }

    /* For e >= 2: multiply by p^(e-2) */
    seq_t pow = 1;
    for (int i = 0; i < e - 2; ++i) pow *= p;

    seq_t res = num * pow;
    /* final division by 6 (exact) */
    return res / 6;
}

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
