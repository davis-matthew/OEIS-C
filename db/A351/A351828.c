/*
 * A351828  Sum of the numbers <= n that are either squarefree, a divisor of
 *          n, or both.
 * https://oeis.org/A351828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of numbers <= n that are either squarefree, a divisor
 *            of n, or both
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Predeclared helper for factorization */
#include "../utils/factor_u64.h"

/* Integer square root for idx_t (uint64_t) */
static idx_t int_sqrt(idx_t n) {
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* Möbius function for small integers */
static int mobius(idx_t d) {
    idx_t temp = d;
    int cnt = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > 1) return 0;
            ++cnt;
        }
    }
    if (temp > 1) ++cnt;
    return (cnt % 2 == 0) ? 1 : -1;
}

/* Recursive generation of divisors */
static void dfs_divisors(int idx, seq_t prod, bool non_sq,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, seq_t *sum_common_out) {
    if (idx == cnt) {
        if (non_sq) *sum_common_out += prod;
        return;
    }
    seq_t p_pow = 1;
    for (int f = 0; f <= (int)exps[idx]; ++f) {
        bool new_non_sq = non_sq || (f >= 2);
        dfs_divisors(idx + 1, prod * p_pow, new_non_sq,
                     primes, exps, cnt, sum_common_out);
        p_pow *= (seq_t)primes[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Sum of squarefree numbers <= n */
    seq_t sum_sqf = 0;
    idx_t max_d = int_sqrt(n);
    for (idx_t d = 1; d <= max_d; ++d) {
        seq_t M = (seq_t)(n / ((idx_t)d * (idx_t)d));
        seq_t part = M * (M + 1) / 2;
        seq_t d2 = (seq_t)d * (seq_t)d;
        part *= d2;
        int mu = mobius(d);
        if (mu == 1) sum_sqf += part;
        else if (mu == -1) sum_sqf -= part;
    }

    /* Sum of divisors of n that are not squarefree */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum_common = 0;
    dfs_divisors(0, (seq_t)1, false, primes, exps, cnt, &sum_common);

    return sum_sqf + sum_common;
}
