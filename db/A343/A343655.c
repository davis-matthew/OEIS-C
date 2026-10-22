/*
 * A343655  Number of pairwise coprime sets of divisors of n, where a
 *          singleton is not considered pairwise coprime unless it is {1}.
 * https://oeis.org/A343655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{U⊆P(n)} B(|U|) * Π_{p∈U} e_p – d(n) where P(n) is
 *            the set of distinct prime factors of n, e_p is the exponent of p
 *            in n, B(·) is Bell number, and d(n) is the number of divisors of
 *            n. This counts all pairwise coprime subsets of divisors of n,
 *            with the rule that a singleton is allowed...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static inline uint64_t binom64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __uint128_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return (uint64_t)res;
}

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* divisor count d(n) = Π (e_p + 1) */
    seq_t d = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (seq_t)(exps[i] + 1);

    /* Bell numbers B[0..cnt] */
    seq_t bell[65];
    bell[0] = (seq_t)1;
    for (idx_t m = 1; m <= cnt; ++m) {
        bell[m] = (seq_t)0;
        for (idx_t j = 0; j < m; ++j) {
            uint64_t bin = binom64(m - 1, j);
            bell[m] += bell[j] * ((seq_t)bin);
        }
    }

    /* Sum over all subsets of primes */
    seq_t sum = (seq_t)0;
    idx_t limit = (cnt == 0) ? 1 : ((idx_t)1 << cnt);
    for (idx_t mask = 0; mask < limit; ++mask) {
        seq_t prod = (seq_t)1;
        idx_t sz = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((idx_t)1 << i)) {
                prod *= (seq_t)exps[i];
                sz++;
            }
        }
        sum += prod * bell[sz];
    }

    /* total_all = 2 * sum  (include optional 1) */
    seq_t total_all = sum + sum;   /* 2 * sum */

    /* final result a(n) = total_all – d(n) */
    seq_t result = total_all - d;
    return result;
}
