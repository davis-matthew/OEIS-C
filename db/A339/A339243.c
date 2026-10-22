/*
 * A339243  Number of partitions of n into divisors of n where every part
 *          appears at least 2 times.
 * https://oeis.org/A339243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partition count of n into divisors of n with each part
 *            appearing at least twice
 * Range:     u8 n <= 35, u16 n <= 95, u32 n <= 335, u64 n <= 1259,
 *            u128 n <= 5039, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* ---------- factor n into primes ----------
       primes[] holds distinct prime factors
       exps[]  holds their exponents
       The total number of factors is fcnt   */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &fcnt);

    /* ---------- generate all divisors of n ----------
       store in array 'divs', count 'd_count'   */
    unsigned long long divs[256];
    size_t d_count = 1;
    divs[0] = 1ULL;                 /* start with divisor 1 */

    for (size_t fi = 0; fi < fcnt; ++fi) {
        unsigned long long p = primes[fi];
        unsigned long long mul = 1ULL;
        size_t old_count = d_count;
        for (int e = 1; e <= (int)exps[fi]; ++e) {
            mul *= p;
            for (size_t i = 0; i < old_count; ++i) {
                divs[d_count++] = divs[i] * mul;
            }
        }
    }

    /* ---------- dynamic programming ----------
       dp[s] = number of ways to form sum s using processed divisors so far   */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(len * sizeof(seq_t));

    /* initialize */
    memset(dp, 0, len * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /* process each divisor d */
    for (size_t di = 0; di < d_count; ++di) {
        unsigned long long d_ll = divs[di];
        idx_t d = (idx_t)d_ll;   /* divisor as index type */

        /* reset next array to zero (will be overwritten anyway) */
        memset(next, 0, len * sizeof(seq_t));

        /* for each residue r modulo d */
        for (idx_t r = 0; r < d; ++r) {
            seq_t sum = (seq_t)0;
            idx_t j = 0;
            idx_t s = r;

            while (s <= n) {
                if (j >= 2) {
                    idx_t prev = r + (j - 2) * d;
                    sum += dp[prev];
                }
                next[s] = dp[s] + sum;
                ++j;
                s = r + j * d;
            }
        }

        /* swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(next);
    return result;
}
