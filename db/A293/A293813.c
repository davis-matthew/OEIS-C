/*
 * A293813  Number of partitions of n into nontrivial divisors of n.
 * https://oeis.org/A293813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into nontrivial divisors of n
 *            Computed by dynamic programming per n using coin‑change dynamic
 *            programming
 * Range:     u8 n <= 35, u16 n <= 119, u32 n <= 335, u64 n <= 1079,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static size_t dp_cap = 0;

static void ensure_dp_size(size_t needed)
{
    if (dp_cap < needed)
    {
        seq_t *newdp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
        if (!newdp) exit(1);  /* allocation failed */
        dp = newdp;
        dp_cap = needed;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    size_t sz = (size_t)n + 1;
    ensure_dp_size(sz);

    /* initialize dp array */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i)
        dp[i] = (seq_t)0;

    /* collect nontrivial divisors of n (strictly between 1 and n) */
    idx_t parts[256];
    int part_cnt = 0;
    for (idx_t d = 2; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            parts[part_cnt++] = d;
            idx_t other = n / d;
            if (other != d)
                parts[part_cnt++] = other;
        }
    }

    /* coin‑change DP: iterate over parts */
    for (int pi = 0; pi < part_cnt; ++pi)
    {
        idx_t part = parts[pi];
        for (idx_t j = part; j <= n; ++j)
        {
            dp[j] = dp[j] + dp[j - part];
        }
    }

    return dp[n];
}
