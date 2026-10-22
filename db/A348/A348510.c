/*
 * A348510  a(n) = A099377(n) - n, where A099377(n) is the numerator of the
 *          harmonic mean of the divisors of n.
 * https://oeis.org/A348510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = numerator of τ(n) / σ_{-1}(n) - n, where τ(n) is the
 *            number of divisors of n and σ_{-1}(n) = Σ_{d|n} 1/d. The
 *            numerator is taken in lowest terms
 * Range:     i8 n <= 9, i16 n <= 256, i32 n <= 9482, i64 n <= 9482,
 *            i128 n <= 9482, big any n
 * Helpers:   gcd, pow_uint_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow_uint_uint.h"

/* Sequence value a(n) as defined. */
seq_t a(idx_t n)
{
    if (n == 0)   /* not used but defensive */
        return 0;

    /* Factorisation of n and building of num/den of the harmonic mean. */
    idx_t remaining = n;
    seq_t num = 1;   /* numerator of H before reduction */
    seq_t den = 1;   /* denominator of H before reduction */

    /* Handle factor 2 separately. */
    if (remaining % 2ULL == 0ULL) {
        idx_t e = 0;
        while (remaining % 2ULL == 0ULL) {
            remaining /= 2ULL;
            ++e;
        }
        seq_t ppow_e   = pow_uint_uint(2ULL, e);    /* 2^e */
        seq_t ppow_ep1 = ppow_e * (seq_t)2ULL;      /* 2^(e+1) */
        seq_t Np = ppow_e * (seq_t)(2ULL - 1ULL) * (seq_t)(e + 1ULL);
        seq_t Dp = ppow_ep1 - (seq_t)1;
        num *= Np;
        den *= Dp;
    }

    /* Odd prime factors. */
    for (idx_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0ULL) {
            idx_t e = 0;
            while (remaining % p == 0ULL) {
                remaining /= p;
                ++e;
            }
            seq_t ppow_e   = pow_uint_uint(p, e);       /* p^e */
            seq_t ppow_ep1 = ppow_e * (seq_t)p;         /* p^(e+1) */
            seq_t Np = ppow_e * (seq_t)(p - 1ULL) * (seq_t)(e + 1ULL);
            seq_t Dp = ppow_ep1 - (seq_t)1;
            num *= Np;
            den *= Dp;
        }
    }

    /* Remaining prime > sqrt(n). */
    if (remaining > 1ULL) {
        idx_t p = remaining;
        idx_t e = 1;
        seq_t ppow_e   = (seq_t)p;                       /* p^1 */
        seq_t ppow_ep1 = ppow_e * (seq_t)p;              /* p^2 */
        seq_t Np = ppow_e * (seq_t)(p - 1ULL) * (seq_t)(e + 1ULL);
        seq_t Dp = ppow_ep1 - (seq_t)1;
        num *= Np;
        den *= Dp;
    }

    /* Reduce the fraction. */
    seq_t g = gcd(num, den);
    num /= g;   /* den / g is not needed afterwards */

    /* Return numerator minus n. */
    return num - (seq_t)n;
}
