/*
 * A341305  Fourier coefficients of the modular form
 *          (1/28)*(E_6(t)+27*E_6(3*t)).
 * https://oeis.org/A341305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 * a(n) = -18 * σ₅(n) for 3∤n * a(n) = -18 * σ₅(n) - 486
 *            * σ₅(n/3) for 3|n * where σ₅(m) = sum of the 5th powers of the
 *            positive divisors of m
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute σ₅(m) by prime factorisation. */
static seq_t sigma5(uint64_t m) {
    seq_t result = 1;
    uint64_t n = m;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            seq_t term   = 1;            /* 1 + p⁵ + p¹² + … */
            seq_t pow5   = (seq_t)p;     /* p¹ */
            for (int i = 0; i < 4; ++i)  /* multiply by p⁴ → p⁵ */
                pow5 *= (seq_t)p;
            seq_t current = 1;
            while (n % p == 0) {
                n /= p;
                current *= pow5;         /* current = p^(5k) */
                term += current;
            }
            result *= term;
        }
    }
    if (n > 1) {                       /* remaining prime factor */
        seq_t term = 1;
        seq_t pow5 = (seq_t)n;
        for (int i = 0; i < 4; ++i)
            pow5 *= (seq_t)n;
        term += pow5;
        result *= term;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t sigma_n = sigma5((uint64_t)n);
    seq_t res = -((seq_t)18 * sigma_n);
    if (n % 3 == 0) {
        seq_t sigma_k = sigma5((uint64_t)(n / 3));
        res -= ((seq_t)486 * sigma_k);
    }
    return res;
}
