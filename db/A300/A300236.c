/*
 * A300236  Möbius transform of A032742 (the largest proper divisor of n).
 * https://oeis.org/A300236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A008683(n/d)*A032742(d) = Sum_{d|n} mu(n/d) *
 *            (d==1 ? 1 : d / smallest_prime_factor_of(d)) where A032742(d) is
 *            the largest proper divisor of d. The function factors n,
 *            enumerates all divisors d, computes mu(n/d), and adds or
 *            subtracts the largest proper divisor accordingly
 * Range:     u8 n <= 512, u16 n <= 9699, u32 n <= 9699, u64 n <= 9699,
 *            u128 n <= 9699, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void dfs_divisors(int idx, int k, const int *pr, const int *exps,
                         seq_t cur_d, int mu, uint64_t spd,
                         seq_t *sum_pos, seq_t *sum_neg)
{
    if (idx == k) {
        seq_t lpd;
        if (cur_d == (seq_t)1) {
            lpd = (seq_t)1;
        } else {
            lpd = cur_d / (seq_t)spd;
        }
        if (mu > 0) {
            *sum_pos += lpd;
        } else if (mu < 0) {
            *sum_neg += lpd;
        }
        return;
    }

    int p = pr[idx];
    int e = exps[idx];
    seq_t d = cur_d;

    for (int f = 0; f <= e; ++f) {
        int ti = e - f;           /* exponent of p in n/d  */
        int mu_next = mu;
        if (mu_next != 0) {
            if (ti >= 2) {
                mu_next = 0;
            } else if (ti == 1) {
                mu_next = -mu_next;
            } /* else ti == 0 => unchanged */
        }
        uint64_t spd_next = spd;
        if (spd == 0 && f > 0) {
            spd_next = (uint64_t)p;   /* first prime factor becomes smallest */
        }

        dfs_divisors(idx + 1, k, pr, exps, d, mu_next, spd_next,
                     sum_pos, sum_neg);

        /* prepare d for next exponent */
        d *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    /* Factor n into primes */
    uint64_t m = (uint64_t)n;
    int primes[20];
    int exps[20];
    int k = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[k] = (int)p;
            exps[k] = e;
            ++k;
        }
    }
    if (m > 1) {
        primes[k] = (int)m;
        exps[k] = 1;
        ++k;
    }

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    dfs_divisors(0, k, primes, exps, (seq_t)1, 1, (uint64_t)0, &sum_pos, &sum_neg);

    return sum_pos - sum_neg;
}
