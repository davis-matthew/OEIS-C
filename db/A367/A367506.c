/*
 * A367506  a(n) = Sum_{d|n} (d+n)^n.
 * https://oeis.org/A367506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d+n)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiply a divisor by successive powers of a prime during recursion. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Recursive helper to sum the terms for all divisors of n. */
static void _sum_divisors(uint64_t *primes, int *exps, size_t pf_cnt,
                          size_t idx, uint64_t current, seq_t n_val,
                          seq_t *sum)
{
    if (idx == pf_cnt) {                    /* finished building one divisor */
        seq_t base = (seq_t)current + n_val; /* (d + n) as seq_t */
        seq_t term = seq_pow(base, (idx_t)n_val); /* (d+n)^n */
        *sum += term;
        return;
    }

    uint64_t p = primes[idx];
    int emax = exps[idx];
    uint64_t cur = current;
    for (int e = 0; e <= emax; ++e) {
        _sum_divisors(primes, exps, pf_cnt, idx + 1, cur, n_val, sum);
        cur *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1, safety */

    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t pf_cnt = 0;

    /* Trial division factorization (fast enough for n <= 10000). */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        primes[pf_cnt] = tmp;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    seq_t total = 0;
    seq_t n_val = (seq_t)n;
    _sum_divisors(primes, exps, pf_cnt, 0, 1ULL, n_val, &total);

    return total;
}
