/*
 * A246742  Number of length 5+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A246742
   Formula: a(n) = 3*a(n-1) +4*a(n-2) -20*a(n-3) +56*a(n-5) -28*a(n-6) -84*a(n-7) +70*a(n-8) +70*a(n-9)
               -84*a(n-10) -28*a(n-11) +56*a(n-12) -20*a(n-14) +4*a(n-15) +3*a(n-16) -a(n-17)
   Initial values for 1 ≤ n ≤ 17 are obtained by a direct DP from the definition. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* -------------------------------------------------------------------- */
static seq_t dp_base_value(idx_t n)
{
    idx_t mod = n + 1;                       // values 0 .. n inclusive
    size_t state_cnt = (size_t)mod * mod * mod * mod;   // 4‑tuples

    seq_t *dp = mem_new(seq_t, state_cnt);
    seq_t *new_dp = mem_new(seq_t, state_cnt);
    if (!dp || !new_dp)
        abort();

    /* initialise states: all 4‑tuples with no forbidden pair among themselves */
    for (size_t a = 0; a < mod; ++a)
        for (size_t b = 0; b < mod; ++b)
            for (size_t c = 0; c < mod; ++c)
                for (size_t d = 0; d < mod; ++d) {
                    size_t key = ((a * mod + b) * mod + c) * mod + d;
                    bool good = true;
                    size_t arr[4] = { a, b, c, d };
                    for (int i = 0; i < 4 && good; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (arr[i] + arr[j] == n)
                                good = false;
                    if (good)
                        dp[key] = 1;
                }

    /* perform 5 transitions to reach length 9 (positions 4..8). */
    for (size_t step = 0; step < 5; ++step) {
        for (size_t i = 0; i < state_cnt; ++i)
            new_dp[i] = 0;

        for (size_t key = 0; key < state_cnt; ++key) {
            seq_t cnt = dp[key];
            if (!cnt)
                continue;

            /* decode the 4‑tuple */
            size_t tmp = key;
            size_t d = tmp % mod; tmp /= mod;
            size_t c = tmp % mod; tmp /= mod;
            size_t b = tmp % mod; tmp /= mod;
            size_t a = tmp;

            size_t vals[4] = { a, b, c, d };

            /* compute which x are forbidden: x = n - k for k in vals */
            bool forbidden[18] = { 0 };                 /* max mod is 18 (n≤17) */
            for (int k = 0; k < 4; ++k) {
                seq_t val = n - vals[k];
                if (val <= n)
                    forbidden[(size_t)val] = true;
            }

            /* try all possible new elements */
            for (size_t x = 0; x < mod; ++x) {
                if (forbidden[(size_t)x])
                    continue;
                size_t new_key = ((b * mod + c) * mod + d) * mod + x;
                new_dp[new_key] += cnt;
            }
        }
        seq_t *tmp = dp;
        dp = new_dp;
        new_dp = tmp;
    }

    /* sum all counts in dp to get the answer */
    seq_t total = 0;
    for (size_t i = 0; i < state_cnt; ++i)
        total += dp[i];

    mem_free(dp);
    mem_free(new_dp);
    return total;
}

/* -------------------------------------------------------------------- */
/*  Cache structure                                                    */
/* -------------------------------------------------------------------- */
static seq_t *cache = NULL;      /* 1‑based cache of computed terms */
static idx_t cache_len = 0;      /* highest n computed so far  */
static size_t cache_cap = 0;     /* allocated capacity (indices 0..cache_cap) */

/* Ensure the cache can store indexes up to n (1‑based).  */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < (size_t)n)
        new_cap *= 2;

    seq_t *ptr = mem_resize(seq_t, cache, (new_cap + 1));
    if (!ptr)
        abort();
    /* initialise new slots to 0 */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i)
        ptr[i] = 0;
    cache = ptr;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/*  DP for n ≤ 17  –  counts arrays 0..n avoiding a pair summing to n    */

/* -------------------------------------------------------------------- */
/*  Fill cache for n = 1..17                                           */
/* -------------------------------------------------------------------- */
static void fill_base_cache(void)
{
    ensure_cache_size(17);
    for (idx_t i = 1; i <= 17; ++i)
        cache[i] = dp_base_value(i);
    cache_len = 17;
    cache[0] = 0;      /* convenience for recurrence */
}

/* -------------------------------------------------------------------- */
/*  Public function                                                    */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (!cache)
        fill_base_cache();       /* initialise base values */

    if (n <= cache_len)
        return cache[n];

    ensure_cache_size(n);

    /* compute terms from cache_len+1 up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos, neg, res;

        /* positive contributions */
        pos = 0;
        pos += cache[i - 1] * (seq_t)3;
        pos += cache[i - 2] * (seq_t)4;
        pos += cache[i - 5] * (seq_t)56;
        pos += cache[i - 8] * (seq_t)70;
        pos += cache[i - 9] * (seq_t)70;
        pos += cache[i -12] * (seq_t)56;
        pos += cache[i -15] * (seq_t)4;
        pos += cache[i -16] * (seq_t)3;

        /* negative contributions */
        neg = 0;
        neg += cache[i - 3] * (seq_t)20;
        neg += cache[i - 6] * (seq_t)28;
        neg += cache[i - 7] * (seq_t)84;
        neg += cache[i -10] * (seq_t)84;
        neg += cache[i -11] * (seq_t)28;
        neg += cache[i -14] * (seq_t)20;
        neg += cache[i -17] * (seq_t)1;

        /* final value (positive ≥ negative by construction) */
        res = pos - neg;
        cache[i] = res;
    }

    cache_len = n;
    return cache[n];
}
