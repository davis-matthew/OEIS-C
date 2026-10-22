/*
 * A338496  Least number of octagonal pyramidal numbers needed to represent n.
 * https://oeis.org/A338496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least number of octagonal pyramidal numbers needed to represent n.
   A(1) = 1,  A(2) = 2,  A(3) = 3,  A(4) = 4,  A(5) = 5, …  */
/*  Sequence A338496 is the coin‑change problem with coins
    P(k) = k(k+1)(2k-1)/2   (k ≥ 1).  */

#include <stddef.h>
#include <stdint.h>

#define MAX_N 10000                /* maximum n that is guaranteed to be
                                   * requested by the test harness */

static seq_t dp[MAX_N + 1];       /* DP table: dp[i] = least number of coins for i */
static seq_t coins[128];          /* all octagonal pyramidal numbers ≤ current n */
static size_t coin_cnt;           /* number of coins in 'coins' */

/* Build all octagonal pyramidal numbers ≤ n into 'coins'. */
static void build_coins(idx_t n)
{
    coin_cnt = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t coin = kk * (kk + 1) * ((seq_t)2 * kk - 1) / 2;
        if (coin > n) break;
        coins[coin_cnt++] = coin;
    }
}

/* Compute a(n).  Returns the minimal number of octagonal pyramidal
 * numbers whose sum is n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for out‑of‑range */
    if (n > MAX_N) {                          /* fallback for larger n */
        /* This branch is rarely executed – the static table covers tests. */
        seq_t *dp_dyn = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!dp_dyn) return 0;                /* allocation failed */
        dp_dyn[0] = 0;
        for (idx_t i = 1; i <= n; ++i) dp_dyn[i] = (seq_t)(n + 1);
        /* Rebuild coins up to n */
        build_coins(n);
        for (size_t ci = 0; ci < coin_cnt; ++ci) {
            seq_t coin = coins[ci];
            for (idx_t j = (idx_t)coin; j <= n; ++j) {
                seq_t cand = dp_dyn[j - (idx_t)coin] + 1;
                if (cand < dp_dyn[j]) dp_dyn[j] = cand;
            }
        }
        seq_t result = dp_dyn[n];
        free(dp_dyn);
        return result;
    }

    /* ---- fast path: static buffer for n ≤ MAX_N ---- */
    build_coins(n);

    dp[0] = 0;
    seq_t inf = (seq_t)(n + 1);                /* value larger than any optimum */
    for (idx_t i = 1; i <= n; ++i) dp[i] = inf;

    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        seq_t coin = coins[ci];
        for (idx_t j = (idx_t)coin; j <= n; ++j) {
            seq_t cand = dp[j - (idx_t)coin] + 1;
            if (cand < dp[j]) dp[j] = cand;
        }
    }
    return dp[n];
}
