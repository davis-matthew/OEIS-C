/*
 * A262780  Expansion of phi(-x^6) * psi(x^4) + x * phi(-x^2) * psi(x^12) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A262780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n + 1) where b() is multiplicative with * b(2^e) =
 *            0^e (i.e. 0 for e>=1, 1 for e==0) * b(3^e) = 1 * b(p^e) = e+1 if
 *            p ≡ 1 or 19 (mod 24) * b(p^e) = (−1)^e (e+1) if p ≡ 7 or 13 (mod
 *            24) * b(p^e) = (1 + (−1)^e) / 2 if p ≡ 5 (mod 6) * (for all
 *            other primes p, the above cases cover all pos...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Macros for larger integer types are taken care of by the build system */
seq_t a(idx_t n) {
    /* m = 2*n + 1 (always odd) */
    uint64_t m = (uint64_t)2 * (uint64_t)n + 1ULL;
    uint64_t temp = m;
    seq_t result = 1;           /* multiplicative identity */

    /* Trial division by odd numbers (2 never divides m) */
    for (uint64_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            /* Count exponent e of prime d */
            int e = 0;
            while (temp % d == 0) {
                temp /= d;
                e++;
            }

            /* Compute b(d^e) according to the rules */
            seq_t factor_value;
            if (d == 3) {
                factor_value = 1;               /* b(3^e) = 1 */
            } else if (d % 24 == 1 || d % 24 == 19) {
                factor_value = (seq_t)(e + 1);   /* e + 1 */
            } else if (d % 24 == 7 || d % 24 == 13) {
                /* (−1)^e (e+1) */
                if (e % 2 == 0) {
                    factor_value = (seq_t)(e + 1);
                } else {
                    factor_value = (seq_t)(-(e + 1));
                }
            } else if (d % 6 == 5) {
                /* (1 + (−1)^e) / 2 : 1 if e even, 0 if e odd */
                factor_value = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            } else {
                /* This path should never be reached for primes > 3 */
                factor_value = 1;
            }

            if (factor_value == 0) {
                return 0;                       /* product becomes zero */
            }
            result *= factor_value;
            if (result == 0) {
                return 0;                       /* early exit if zero */
            }
        }
    }

    /* If a prime factor > sqrt(m) remains */
    if (temp > 1) {
        uint64_t p = temp;
        int e = 1;
        seq_t factor_value;
        if (p == 3) {
            factor_value = 1;
        } else if (p % 24 == 1 || p % 24 == 19) {
            factor_value = (seq_t)(e + 1);
        } else if (p % 24 == 7 || p % 24 == 13) {
            if (e % 2 == 0) {
                factor_value = (seq_t)(e + 1);
            } else {
                factor_value = (seq_t)(-(e + 1));
            }
        } else if (p % 6 == 5) {
            factor_value = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            factor_value = 1;
        }
        if (factor_value == 0) {
            return 0;
        }
        result *= factor_value;
    }

    return result;
}
