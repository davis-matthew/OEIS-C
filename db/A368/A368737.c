/*
 * A368737  a(n) = Sum_{k = 1..n} gcd(3*k, n).
 * https://oeis.org/A368737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k = 1..n} gcd(3*k, n)
 * Range:     u8 n <= 35, u16 n <= 1889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9874 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper that factors a 64‑bit integer: factor_u64(n, primes,
   exponents, count). The header is provided by the build system. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive routine that walks all divisors d of N and accumulates
   (N/d)*phi(d), where phi(d) is the Euler totient of d. */
static void dfs_sum_gcd(size_t i, seq_t div, seq_t phi, size_t cnt,
                        uint64_t *pr, uint64_t *ex, seq_t NN, seq_t *sum)
{
    if (i == cnt) {
        /* Contribution of the divisor 'div'. */
        *sum += (NN / div) * phi;
        return;
    }
    uint64_t p = pr[i];
    uint64_t e = ex[i];
    seq_t pow = 1;
    for (uint64_t exp = 0; exp <= e; ++exp) {
        seq_t new_div = div * pow;
        seq_t new_phi = phi;
        if (exp > 0) {
            /* phi(p^exp) = p^(exp-1)*(p-1) */
            new_phi = new_phi * (pow / p) * (seq_t)(p - 1);
        }
        dfs_sum_gcd(i + 1, new_div, new_phi, cnt, pr, ex, NN, sum);
        pow *= (seq_t)p;
    }
}

/* Computes S(N) = Sum_{k=1..N} gcd(k, N) */
static seq_t sum_gcd(idx_t N)
{
    if (N == 0) return 0;
    /* Factorize N. 64 prime slots are more than enough for all N < 2^64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)N, primes, exps, &cnt);

    seq_t NN = (seq_t)N;
    seq_t total = 0;
    dfs_sum_gcd(0, (seq_t)1, (seq_t)1, cnt, primes, exps, NN, &total);
    return total;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n % 3 == 0) {
        idx_t m = n / 3;
        return (seq_t)9 * sum_gcd(m);
    } else {
        return sum_gcd(n);
    }
}
