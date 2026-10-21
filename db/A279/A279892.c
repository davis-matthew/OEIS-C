/*
 * A279892  Eisenstein series E_18(q) (alternate convention E_9(q)),
 *          multiplied by 43867.
 * https://oeis.org/A279892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   For n = 0, a(0) = 43867; for n >= 1, a(n) = -28728 * sigma_17(n)
 *            where sigma_17(n) = sum_{d|n} d^17
 * Range:     i8 none, i16 none, i32 n <= 1, i64 n <= 7, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Computes base^exp as a seq_t using repeated squaring. */
static inline seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)43867;

    seq_t sigma = 1;
    idx_t m = n;

    /* Factorisation by trial division (sufficient for n <= 10000). */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* Compute 1 + p^17 + p^34 + ... + p^(17·e) without overflow. */
            seq_t term = 1;
            seq_t pow = 1;
            seq_t p17  = pow_seq_local((seq_t)p, (idx_t)17);
            for (idx_t i = 0; i < e; ++i) {
                pow *= p17;      /* pow = p^(17·(i+1)) */
                term += pow;
            }

            sigma *= term;
        }
    }

    /* Remaining prime factor (if n had a prime > sqrt(original n)). */
    if (m > 1) {
        /* For a prime factor to the power 1: 1 + m^17. */
        seq_t p17 = pow_seq_local((seq_t)m, (idx_t)17);
        sigma *= (1 + p17);
    }

    return -(seq_t)28728 * sigma;
}
