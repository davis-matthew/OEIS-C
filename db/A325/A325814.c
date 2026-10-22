/*
 * A325814  a(n) = n - A048146(n), where A048146 is the sum of non-unitary
 *          divisors of n.
 * https://oeis.org/A325814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A048146(n) = n - (sum of non‑unitary divisors)
 * Range:     i8 n <= 12, i16 n <= 2341, i32 n <= 9758, i64 n <= 9758,
 *            i128 n <= 9758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial division factorization. */
#include <stdint.h>
#include <stddef.h>

/* integer power for seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

/* factorize n into primes and exponents (at most 20 distinct primes) */
static void factor_uint64(idx_t n, uint64_t primes[20], int exps[20], size_t *cnt) {
    size_t i = 0;
    idx_t m = n;
    for (idx_t p = (idx_t)2; p <= m / p; ++p) {
        if (m % p == (idx_t)0) {
            int e = 0;
            while (m % p == (idx_t)0) {
                m /= p;
                ++e;
            }
            primes[i] = (uint64_t)p;
            exps[i]   = e;
            ++i;
        }
    }
    if (m > (idx_t)1) {
        primes[i] = (uint64_t)m;
        exps[i]   = 1;
        ++i;
    }
    *cnt = i;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* offset starts at 1 */

    uint64_t primes[20];
    int      exps[20];
    size_t   cnt;
    factor_uint64(n, primes, exps, &cnt);

    seq_t unit_sum  = (seq_t)1;
    seq_t total_sum = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        /* p^e */
        seq_t p_pow = pow_seq(p, e);

        /* unitary divisor sum factor (1 + p^e) */
        unit_sum *= ((seq_t)1 + p_pow);

        /* sum of all divisors of p^e: (p^{e+1} - 1)/(p - 1) */
        seq_t p_pow_e1 = p_pow * p;
        seq_t numerator   = p_pow_e1 - (seq_t)1;
        seq_t denominator = p - (seq_t)1;
        seq_t divisor_sum = numerator / denominator;

        total_sum *= divisor_sum;
    }

    seq_t non_unit_sum = total_sum - unit_sum;
    return ((seq_t)n) - non_unit_sum;
}
