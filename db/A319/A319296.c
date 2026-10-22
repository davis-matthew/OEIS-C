/*
 * A319296  a(n) = (Sum_{d|n} sigma(d)) mod sigma(n).
 * https://oeis.org/A319296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Σ_{d|n} sigma(d)) mod sigma(n)
 * Range:     u8 n <= 161, u16 n <= 9505, u32 n <= 9505, u64 n <= 9505,
 *            u128 n <= 9505, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computing sigma(d) and sigma(n) via the provided utility
   functions "sigma_of" and "factor_u64".  This routine enumerates
   all divisors of n by recursive DFS over its prime factorisation. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* Recursive helper: enumerate all divisors of n and accumulate
   Σ sigma(d) modulo sigma_n.  `current` tracks the current divisor
   being constructed. */
static void dfs_divisors(size_t idx,
                         uint64_t current,
                         const uint64_t *prime,
                         const int *exp,
                         size_t num,
                         seq_t sigma_n,
                         seq_t *sum)
{
    if (idx == num) {
        /* `current` is a divisor of n; its sigma is needed. */
        seq_t s = sigma_of((idx_t)current);
        *sum = (*sum + s) % sigma_n;
        return;
    }
    uint64_t power = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        dfs_divisors(idx + 1, current * power, prime, exp, num,
                     sigma_n, sum);
        power *= prime[idx];
    }
}

/* Main sequence function: a(n) */
seq_t a(idx_t n)
{
    /* n ≥ 1 by specification.  Compute σ(n). */
    seq_t sigma_n = sigma_of(n);
    /* σ(n) cannot be zero for n ≥ 1, but guard against potential
       misuse of the function. */
    if (sigma_n == 0) return 0;

    /* Factorise n.  64 primes is sufficient for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t factor_cnt;
    factor_u64((uint64_t)n, primes, exponents, &factor_cnt);

    /* Convert exponents to int for array indexing. */
    int exp_int[64];
    for (size_t i = 0; i < factor_cnt; ++i)
        exp_int[i] = (int)exponents[i];

    seq_t sum = 0;
    dfs_divisors(0, (uint64_t)1, primes, exp_int,
                 factor_cnt, sigma_n, &sum);
    return sum;  /* sum already reduced modulo σ(n) */
}
