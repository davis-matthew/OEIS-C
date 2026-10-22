/*
 * A348985  Numerator of ratio sigma(n) / A325973(n), where A325973 is the
 *          arithmetic mean of {sum of squarefree divisors} and {sum of
 *          unitary divisors}.
 * https://oeis.org/A348985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd( sigma(n), A325973(n) ) where A325973(n) =
 *            (sum_of_squarefree_divisors(n) + sum_of_unitary_divisors(n)) /
 *            2. The numerator can be computed as a(n) = 2 * sigma(n) / gcd( 2
 *            * sigma(n), sum_sf + sum_uni ), with the products taken over the
 *            prime factorisation of n
 * Range:     u8 n <= 22, u16 n <= 3364, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* local helper to compute powers of a prime */
static seq_t pow_seq_t(seq_t base, int exp) {
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n = 0, but returns 0 */

    idx_t x = n;
    seq_t sigma = 1;        /* sum of all divisors */
    seq_t sum_sf = 1;       /* sum of square‑free divisors */
    seq_t sum_uni = 1;      /* sum of unitary divisors */

    /* process factor 2 separately */
    if (x % 2 == 0) {
        int exp = 0;
        while (x % 2 == 0) {
            x /= 2;
            exp++;
        }
        seq_t p_pow = pow_seq_t(2, exp);          /* 2^exp */
        seq_t p_pow_next = p_pow * 2;            /* 2^(exp+1) */
        seq_t term_sigma = (p_pow_next - 1) / 1; /* (2^(exp+1)-1)/(2-1) */
        sigma *= term_sigma;
        sum_sf *= (1 + (seq_t)2);
        sum_uni *= (1 + p_pow);
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            seq_t p_pow = pow_seq_t((seq_t)p, exp);       /* p^exp */
            seq_t p_pow_next = p_pow * (seq_t)p;          /* p^(exp+1) */
            seq_t term_sigma = (p_pow_next - 1) / ((seq_t)p - 1);
            sigma *= term_sigma;
            sum_sf *= (1 + (seq_t)p);
            sum_uni *= (1 + p_pow);
        }
    }

    /* remaining prime > sqrt(original n ) */
    if (x > 1) {
        seq_t p = (seq_t)x;            /* the prime factor */
        seq_t p_pow = p;               /* exponent 1 */
        seq_t p_pow_next = p_pow * p;  /* p^2 */
        seq_t term_sigma = (p_pow_next - 1) / (p - 1);
        sigma *= term_sigma;
        sum_sf *= (1 + p);
        sum_uni *= (1 + p_pow);
    }

    seq_t two_sigma = sigma * 2;
    seq_t sum_sf_plus_uni = sum_sf + sum_uni;
    seq_t g = gcd(two_sigma, sum_sf_plus_uni);
    seq_t result = two_sigma / g;
    return result;
}
