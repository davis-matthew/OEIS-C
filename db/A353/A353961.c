/*
 * A353961  a(n) = Sum_{d|n} d' * (-1)^(d'), where d' is the arithmetic
 *          derivative of d (A003415).
 * https://oeis.org/A353961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} d' * (-1)^(d') (where d' is the arithmetic
 *            derivative of d)
 * Range:     i8 n <= 47, i16 n <= 3071, i32 n <= 8602, i64 n <= 8602,
 *            i128 n <= 8602, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple trial‑division factorisation – sufficient for n ≤ 10000. */
static void factorize_uint64(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
        73,79,83,89,97        /* sqrt(10000) ≈ 100 */
    };
    int idx = 0;
    *cnt = 0;
    for (int i = 0; i < 25 && small_primes[i] * small_primes[i] <= n; ++i) {
        int e = 0;
        while (n % small_primes[i] == 0) {
            n /= small_primes[i];
            ++e;
        }
        if (e > 0) {
            primes[*cnt]   = small_primes[i];
            exps[*cnt]     = e;
            ++(*cnt);
        }
    }
    if (n > 1) { /* remaining prime factor */
        primes[*cnt]   = n;
        exps[*cnt]     = 1;
        ++(*cnt);
    }
}

/* Recursively enumerate all divisors of n.  For each divisor d we add
   d' * (-1)^(d') to the running sum. */
static void enumerate_divisors(int idx, uint64_t cur_d, int *exp_div,
                               const uint64_t *pr, const int *ex, int cnt,
                               int64_t *sum)
{
    if (idx == cnt) {                 /* reached a full divisor */
        int64_t d_prime = 0;
        for (int i = 0; i < cnt; ++i) {
            int e = exp_div[i];
            if (e == 0) continue;
            /* d / p_i is exact division */
            uint64_t d_div_p = cur_d / pr[i];
            d_prime += (int64_t)e * (int64_t)d_div_p;
        }
        int64_t sign = (d_prime & 1) ? -1 : 1;
        *sum += d_prime * sign;
        return;
    }

    /* e = 0 case */
    enumerate_divisors(idx + 1, cur_d, exp_div, pr, ex, cnt, sum);

    /* e > 0 cases */
    uint64_t p_pow = 1;
    for (int e = 1; e <= ex[idx]; ++e) {
        p_pow *= pr[idx];
        uint64_t d_new = cur_d * p_pow;
        exp_div[idx] = e;
        enumerate_divisors(idx + 1, d_new, exp_div, pr, ex, cnt, sum);
    }
    exp_div[idx] = 0;   /* reset for caller */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset – return 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int cnt;

    factorize_uint64(nn, primes, exps, &cnt);

    int64_t acc = 0;
    int exp_div[64] = {0};

    enumerate_divisors(0, (uint64_t)1, exp_div, primes, exps, cnt, &acc);

    return (seq_t)acc;
}
