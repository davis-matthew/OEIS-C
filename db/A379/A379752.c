/*
 * A379752  Number of pairs (d, k/d), d | k, d < k/d, such that gcd(d, k/d) >
 *          1 and neither rad(d) | k/d nor rad(k/d) | d, where k is in
 *          A375055.
 * https://oeis.org/A379752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of pairs (d, k/d) with d | k, d<k/d, gcd(d,k/d)>1,
 *            * rad(d)∤k/d and rad(k/d)∤d, where k = A375055(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* ----- helper: recursively enumerate all divisors of k and count the
 * qualified pairs ----- */
static void dfs_count(uint64_t k,
                      const uint64_t *primes,
                      const uint64_t *exps,
                      size_t num,
                      size_t idx,
                      uint64_t cur_d,
                      int *cur_exp,
                      seq_t *cnt)
{
    if (idx == num) {
        if (cur_d <= 1) return;                /* skip d = 1 */
        uint64_t e = k / cur_d;
        if (cur_d >= e) return;                /* only d < e */

        bool common = false, d_only = false, e_only = false;
        for (size_t i = 0; i < num; ++i) {
            int c = cur_exp[i];
            int r = (int)exps[i] - c;
            if (c > 0 && r > 0) common = true;
            if (c > 0 && r == 0) d_only = true;
            if (c == 0 && r > 0) e_only = true;
            if (common && d_only && e_only) break;
        }
        if (common && d_only && e_only) (*cnt)++;
        return;
    }

    uint64_t p = primes[idx];
    uint64_t power = 1;
    for (int e = 0; e <= (int)exps[idx]; ++e) {
        cur_exp[idx] = e;
        uint64_t next_d = cur_d * power;
        dfs_count(k, primes, exps, num, idx + 1, next_d, cur_exp, cnt);
        if (e < (int)exps[idx]) power *= p;
    }
}

/* Count the number of qualifying pairs for a single k. */
static seq_t count_pairs_uint64(uint64_t k)
{
    if (k < 2) return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t num = 0;
    factor_u64(k, primes, exps, &num);
    if (num == 0) return 0;

    int cur_exp[64] = {0};
    seq_t result = 0;
    dfs_count(k, primes, exps, num, 0, 1, cur_exp, &result);
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    /* Cache of computed a(k) values. */
    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static size_t cache_cap      = 0;
    static uint64_t next_k        = 2;       /* start testing from 2 */

    /* Generate terms until we have at least n values. */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k++;
        seq_t cnt = count_pairs_uint64(k);
        if (cnt > 0) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cnt;
        }
    }

    return cache[n - 1];
}
