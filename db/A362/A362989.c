/*
 * A362989  a(n) = lcm({i + 1, i = 0..n}) / Product_{d | n, d + 1 prime} d.
 * https://oeis.org/A362989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..n+1) / Π{d | n, (d+1) prime} (d+1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64, is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by prime exponents: lcm(1..m)=∏p≤m p^⌊log_p(m)⌋, subtract
   exponents of primes in every (d+1) for d|n with (d+1) prime. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 0–index offset */
    if (n == 0)
        return (seq_t)1;

    /* 1. Build primes up to n+1 and compute lcm exponents */
    const idx_t m = n + 1;                  /* upper bound for lcm */
    size_t primes_cnt = 0;
    uint64_t *primes = NULL;
    /* build_primes_upto expects limit inclusive */
    build_primes_upto(m, &primes, &primes_cnt);
    if (primes_cnt == 0) { /* unlikely for n>=1 */
        free(primes);
        return (seq_t)1;
    }

    /* prime_exponent[i] holds exponent of primes[i] in lcm(1..m) */
    int *prime_exponent = (int *)calloc(primes_cnt, sizeof(int));
    if (!prime_exponent) {
        free(primes);
        return (seq_t)1; /* fallback */
    }

    /* compute floor(log_p(m)) for each prime p <= m */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > m) break;
        int e = 0;
        uint64_t pow = p;
        while (pow <= m) {
            ++e;
            if (pow > m / p) break;  /* avoid overflow */
            pow *= p;
        }
        prime_exponent[i] = e;
    }

    /* 2. Subtract exponents of primes in every (d+1) where d|n and (d+1) prime */
    /* iterate over divisors d of n */
    /* To avoid duplicate divisors for perfect squares, loop i up to sqrt(n) */
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* first divisor d1 = i */
        idx_t d1 = i;
        uint64_t val1 = d1 + 1;
        if (is_prime(val1)) {
            /* factor val1 */
            uint64_t facs[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64(val1, facs, exps, &cnt);
            for (size_t k = 0; k < cnt; ++k) {
                uint64_t pf = facs[k];
                int sub = (int)exps[k];
                /* find index of pf in primes */
                for (size_t j = 0; j < primes_cnt; ++j) {
                    if (primes[j] == pf) {
                        prime_exponent[j] -= sub;
                        break;
                    }
                }
            }
        }

        /* second divisor d2 = n / i, if distinct */
        if (i * i == n) continue;       /* skip double counting if perfect square */

        idx_t d2 = n / i;
        uint64_t val2 = d2 + 1;
        if (is_prime(val2)) {
            uint64_t facs[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64(val2, facs, exps, &cnt);
            for (size_t k = 0; k < cnt; ++k) {
                uint64_t pf = facs[k];
                int sub = (int)exps[k];
                for (size_t j = 0; j < primes_cnt; ++j) {
                    if (primes[j] == pf) {
                        prime_exponent[j] -= sub;
                        break;
                    }
                }
            }
        }
    }

    /* 3. Compose final value from remaining exponents */
    seq_t result = (seq_t)1;
    for (size_t j = 0; j < primes_cnt; ++j) {
        int e = prime_exponent[j];
        if (e > 0) {
            seq_t pow_val = big_pow((seq_t)primes[j], (seq_t)e);
            result = seq_mul(result, pow_val);
        }
    }

    /* clean up */
    free(primes);
    free(prime_exponent);

    return result;
}
