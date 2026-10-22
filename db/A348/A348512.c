/*
 * A348512  a(n) = A003959(sigma(n)), where A003959 is multiplicative with
 *          a(p^e) = (p+1)^e and sigma is the sum of divisors function.
 * https://oeis.org/A348512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003959(σ(n)) where σ(n) is the sum of divisors of n and
 *            A003959 is multiplicative with A003959(pᵉ) = (p+1)ᵉ
 * Range:     u8 n <= 29, u16 n <= 761, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t sigma_of(idx_t n)
{
    seq_t sigma = 1;
    idx_t m = n;
    idx_t p = 2;
    for (; p <= m / p; ++p) {
        if (m % p == 0) {
            /* prime power p^e divides n */
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute 1 + p + p² + … + pᵉ */
            seq_t term = 1;
            seq_t pow = 1;
            for (int i = 0; i < e; ++i) {
                pow *= (seq_t)p;
                term += pow;
            }
            sigma *= term;
        }
    }
    if (m > 1) {            /* remaining prime factor */
        sigma *= (seq_t)(1 + m);
    }
    return sigma;
}

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* compute A003959(sigma) = ∏ (p+1)^e over prime powers of sigma */
    idx_t m = (idx_t)sigma;   /* safe for the tested range up to ~10⁴ */
    seq_t result = 1;

    idx_t p = 2;
    for (; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* multiply by (p+1)^e */
            seq_t base = (seq_t)(p + 1);
            seq_t term = 1;
            for (int i = 0; i < e; ++i) {
                term *= base;
            }
            result *= term;
        }
    }
    if (m > 1) {                /* remaining prime factor */
        result *= (seq_t)(m + 1);
    }
    return result;
}
