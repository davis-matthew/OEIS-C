/*
 * A359166  a(n) = lambda(n) * lambda(sigma(n)), where lambda is Liouville's
 *          lambda, and sigma is the sum of divisors function.
 * https://oeis.org/A359166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = lambda(n) * lambda(sigma(n)) where lambda(m) =
 *            (-1)^{Ω(m)} (Liouville) and sigma is the sum of divisors
 * Range:     i8 n <= 59, i16 n <= 9454, i32 n <= 9454, i64 n <= 9454,
 *            i128 n <= 9454, big any n
 * Helpers:   lambda_of_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/lambda_of_seq.h"

/* Compute σ(n) = sum of divisors of n */
static inline seq_t sigma_of_idx(idx_t n)
{
    seq_t sigma = 1;
    idx_t tmp = n;

    /* factor 2 */
    if (tmp % 2 == 0) {
        int exp = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            exp++;
        }
        seq_t pPow = 1;          /* 2^(exp+1) */
        for (int i = 0; i < exp + 1; ++i)
            pPow *= 2;
        /* (2^(exp+1)-1)/(2-1) = 2^(exp+1)-1 */
        sigma *= (pPow - 1);
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            seq_t pPow = 1;
            for (int i = 0; i < exp + 1; ++i)
                pPow *= (seq_t)p;
            seq_t term = (pPow - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        /* exponent is 1: (p^2-1)/(p-1) = p+1 */
        sigma *= ((seq_t)tmp + 1);
    }

    return sigma;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but harmless */

    seq_t lambda_n     = lambda_of_seq((seq_t)n);
    seq_t sigma_n      = sigma_of_idx(n);
    seq_t lambda_sigma = lambda_of_seq(sigma_n);

    return lambda_n * lambda_sigma;
}
