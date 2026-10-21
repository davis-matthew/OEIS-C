/*
 * A290397  Least number of nonprime squarefree numbers (A000469) that add up
 *          to n.
 * https://oeis.org/A290397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number of nonprime squarefree numbers that sum to n
 *            (OEIS A290397). The function implements a dynamic programming
 *            algorithm that builds the DP table incrementally
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamic data */
static seq_t *dp = NULL;           /* minimal counts: dp[i] = a(i)            */
static idx_t dp_size = 0;          /* current maximum index covered          */
static uint32_t *spf = NULL;       /* smallest prime factor table            */
static size_t spf_size = 0;        /* allocated size of spf array            */
static idx_t *coins = NULL;        /* list of nonprime squarefree numbers    */
static size_t coins_cnt = 0;       /* number of coins                        */
static size_t coins_cap = 0;       /* capacity of the coins array            */

/* add a new coin to the list (respecting C++ cast rules) */
static void add_coin(idx_t c)
{
    if (coins_cnt == coins_cap) {
        size_t new_cap = coins_cap == 0 ? 16 : coins_cap * 2;
        idx_t *tmp = (idx_t *)realloc(coins, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        coins = tmp;
        coins_cap = new_cap;
    }
    coins[coins_cnt++] = c;
}

/* check whether v is squarefree using the spf table */
static bool is_squarefree(idx_t v, const uint32_t *spfin)
{
    if (v <= 1) return true;              /* 1 is squarefree by definition */
    uint32_t prev = 0;
    while (v > 1) {
        uint32_t p = spfin[v];
        if (p == 0) p = v;                /* safety net – should not happen */
        int cnt = 0;
        while (v % p == 0) {
            v /= p;
            ++cnt;
            if (cnt > 1) return false;    /* repeated prime factor detected */
        }
        prev = p;
    }
    return true;
}

/* extend the data structures so that dp and spf are valid for indices ≤ n */
static void extend_to(idx_t n)
{
    if (n <= dp_size) return;                 /* already covered */

    /* remember previous size before resizing */
    idx_t old_max = dp_size;

    /* resize dp array */
    seq_t *tmp_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!tmp_dp) exit(EXIT_FAILURE);
    dp = tmp_dp;
    for (idx_t i = old_max + 1; i <= n; ++i) dp[i] = (seq_t)i;  /* initial value via coin 1 */
    dp[0] = 0;
    dp_size = n;

    /* resize SPF table if necessary */
    if (spf_size < (size_t)(n + 1)) {
        uint32_t *tmp_spf = (uint32_t *)realloc(spf, (n + 1) * sizeof(uint32_t));
        if (!tmp_spf) exit(EXIT_FAILURE);
        spf = tmp_spf;
        spf_size = n + 1;
    }
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;

    /* linear sieve to fill spf up to n */
    size_t primes_cap = 128;
    idx_t *primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || (idx_t)i * p > n) break;
            spf[i * p] = (uint32_t)p;
        }
    }
    free(primes);

    /* ensure coin 1 is present */
    if (coins_cnt == 0) add_coin(1);

    /* add new coins in the range (old_max .. n] */
    for (idx_t val = old_max + 1; val <= n; ++val) {
        if (val == 1) continue;                /* already added */
        if (spf[val] != val && is_squarefree(val, spf)) {
            add_coin(val);
        }
    }

    /* recompute DP for all indices up to n using all current coins */
    for (size_t ci = 0; ci < coins_cnt; ++ci) {
        idx_t c = coins[ci];
        if (c > n) break;
        for (idx_t i = c; i <= n; ++i) {
            seq_t candidate = dp[i - c] + 1;
            if (candidate < dp[i]) dp[i] = candidate;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* The requested function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; sequence is defined for n≥1 */
    extend_to(n);
    return dp[n];
}
