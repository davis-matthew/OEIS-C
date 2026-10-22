/*
 * A326054  a(n) = A326053(n) - n, where A326053 gives the sum of all other
 *          divisors of n except the largest square divisor.
 * https://oeis.org/A326054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sigma(n) - largest_square_divisor(n)) - n
 * Range:     i8 n <= 48, i16 n <= 9514, i32 n <= 9514, i64 n <= 9514,
 *            i128 n <= 9514, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where sigma(n) is the sum of all divisors of n and
    largest_square_divisor(n) is ∏ p^{2·⌊e/2⌋} for each prime p^e || n */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Edge case: n must be positive (offset 1) */
    if (n == 0)
        return 0;  /* undefined for n=0, return 0 */

    /* Work on a mutable copy */
    uint64_t m = (uint64_t)n;

    /* Sum of divisors (sigma) and largest square divisor (sq) */
    seq_t sigma = 1;      /* multiplicative identity */
    seq_t sq    = 1;

    /* Factor 2 separately */
    if ((m & 1ULL) == 0) {
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }

        /* (2^(e+1) - 1) / (2 - 1) = 2^(e+1) - 1 */
        seq_t pow_term = 1;
        for (idx_t i = 0; i < e + 1; ++i)
            pow_term *= (seq_t)2;
        sigma *= (pow_term - 1);

        /* square part */
        idx_t sq_exp = (e / 2) * 2;           /* 2*floor(e/2) */
        if (sq_exp > 0) {
            seq_t pow_sq = 1;
            for (idx_t i = 0; i < sq_exp; ++i)
                pow_sq *= (seq_t)2;
            sq *= pow_sq;
        }
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* sigma contribution: (p^(e+1)-1)/(p-1) */
        seq_t pow_term = 1;
        for (idx_t i = 0; i < e + 1; ++i)
            pow_term *= (seq_t)p;
        seq_t term = (pow_term - 1) / ((seq_t)p - 1);
        sigma *= term;

        /* square part contribution */
        idx_t sq_exp = (e / 2) * 2;
        if (sq_exp > 0) {
            seq_t pow_sq = 1;
            for (idx_t i = 0; i < sq_exp; ++i)
                pow_sq *= (seq_t)p;
            sq *= pow_sq;
        }
    }

    /* If a prime factor > sqrt(n) remains, its exponent is 1 */
    if (m > 1) {
        sigma *= ((seq_t)m + 1);   /* term = (m^2-1)/(m-1) = m+1 because e=1 */
        /* no square contribution as e=1 gives floor(e/2)=0 */
    }

    /* a(n) = sigma - sq - n */
    return sigma - sq - (seq_t)n;
}
