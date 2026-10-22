/*
 * A330225  Position of first appearance of n in A290103 = LCM of prime
 *          indices.
 * https://oeis.org/A330225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the first appearance of n in A290103, i.e.
 *            the smallest m such that the least common multiple of the
 *            indices of the prime factors of m equals n
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_idx, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factor_idx.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    /* Factor n to obtain the required prime powers. */
    idx_t primes_n[16];
    int exp_n[16];
    int cnt_n = 0;
    factor_idx(n, primes_n, exp_n, &cnt_n);

    unsigned full_mask = (1U << cnt_n) - 1U;

    /* DP over subset of prime powers required. */
    /* Since n <= 10000, cnt_n <= 5, so allocate enough space. */
    seq_t dp[128] = {0};
    dp[0] = 1;

    /* Enumerate all divisors of n except 1. */
    idx_t divs[256];
    int div_cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d != 1) divs[div_cnt++] = d;
            idx_t d2 = n / d;
            if (d2 != d && d2 != 1) divs[div_cnt++] = d2;
        }
    }

    /* Process each divisor: determine which prime powers it covers. */
    for (int di = 0; di < div_cnt; ++di) {
        idx_t i = divs[di];
        unsigned mask = 0;
        for (int j = 0; j < cnt_n; ++j) {
            int e = 0;
            idx_t tmp = i;
            while (tmp % primes_n[j] == 0) {
                tmp /= primes_n[j];
                ++e;
            }
            if (e >= exp_n[j]) mask |= (1U << j);
        }
        if (mask == 0) continue;                /* nothing to cover */

        seq_t prime_i = get_prime(i);

        /* 0‑1 knapsack style DP. */
        for (unsigned m = 0; m <= full_mask; ++m) {
            if (dp[m] == 0) continue;
            unsigned nm = m | mask;
            seq_t val = dp[m] * prime_i;
            if (dp[nm] == 0 || val < dp[nm]) dp[nm] = val;
        }
    }

    return dp[full_mask];
}
