/*
 * A350251  Number of non-alternating permutations of the multiset of prime
 *          factors of n.
 * https://oeis.org/A350251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008480(n) - A345164(n) * (Number of permutations of the
 *            multiset of prime factors of n minus * the number of alternating
 *            permutations of that multiset.) * * Uses a brute‑force
 *            enumeration of alternating permutations, which * is feasible for
 *            n <= 10000 because the multiset length is at mos...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

static void dfs_perm(size_t depth, int pattern, const seq_t *primes,
                     size_t *cnt, size_t k, size_t len, seq_t prev,
                     seq_t *out_count)
{
    if (depth == len) {
        (*out_count) += (seq_t)1;
        return;
    }
    for (size_t i = 0; i < k; ++i) {
        if (cnt[i] == 0)
            continue;
        seq_t val = primes[i];
        if (depth > 0) {
            int parity = depth & 1;  /* 1 => odd index (second element) */
            int ok;
            if (pattern == 0) { /* up first: <, >, <, > ... */
                ok = parity ? (prev < val) : (prev > val);
            } else {           /* down first: >, <, >, < ... */
                ok = parity ? (prev > val) : (prev < val);
            }
            if (!ok)
                continue;
        }
        cnt[i]--;
        dfs_perm(depth + 1, pattern, primes, cnt, k, len, val, out_count);
        cnt[i]++;
    }
}

seq_t a(idx_t n)
{
    /* Special case n==1 */
    if (n == 1) return (seq_t)0;

    /* Factor n */
    uint64_t pr[64], ex[64];
    size_t num;
    factor_u64((uint64_t)n, pr, ex, &num);

    /* Total number of prime factors (with multiplicity) */
    size_t total_factors = 0;
    seq_t primes[64];
    size_t counts[64];
    for (size_t i = 0; i < num; ++i) {
        total_factors += (size_t)ex[i];
        primes[i] = (seq_t)pr[i];
        counts[i] = (size_t)ex[i];
    }

    /* Total permutations: factorial(total_factors) / product factorial(ex[i]) */
    seq_t total = fact_upto((idx_t)total_factors);
    for (size_t i = 0; i < num; ++i) {
        total /= fact_upto((idx_t)ex[i]);
    }

    /* Count alternating permutations */
    seq_t up_count = 0, down_count = 0;
    size_t cnt_copy[64];
    for (size_t i = 0; i < num; ++i) cnt_copy[i] = counts[i];

    dfs_perm(0, 0, primes, cnt_copy, num, total_factors, (seq_t)0, &up_count);
    for (size_t i = 0; i < num; ++i) cnt_copy[i] = counts[i];
    dfs_perm(0, 1, primes, cnt_copy, num, total_factors, (seq_t)0, &down_count);

    seq_t alternating = up_count + down_count;
    if (total_factors <= 1) /* avoid double counting for length 0 or 1 */
        alternating -= (seq_t)1;

    return total - alternating;
}
