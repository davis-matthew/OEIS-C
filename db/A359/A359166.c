/*
 * A359166  a(n) = lambda(n) * lambda(sigma(n)), where lambda is Liouville's
 *          lambda, and sigma is the sum of divisors function.
 * https://oeis.org/A359166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = lambda(n) * lambda(sigma(n)) where lambda(m) =
 *            (-1)^{Ω(m)} (Liouville) and sigma is the sum of divisors
 * Range:     i8 n <= 59, i16 n <= 100000, i32 n <= 100000, i64 n <= 100000,
 *            i128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9454 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Liouville λ(m): return +1 for an even total exponent count, -1 otherwise */
static inline seq_t lambda_of_seq(seq_t m)
{
    if (m <= 1)
        return 1;                /* lambda(1) = 1 */

    int parity = 0;              /* 0 = even, 1 = odd */

    /* factor 2 */
    if ((m & 1) == 0) {
        int cnt = 0;
        while ((m & 1) == 0) {
            m >>= 1;            /* divide by 2 */
            cnt++;
        }
        if (cnt & 1)
            parity ^= 1;
    }

    /* factor odd primes */
    for (seq_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            if (cnt & 1)
                parity ^= 1;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1)
        parity ^= 1;

    return (parity == 0) ? 1 : -1;
}

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
