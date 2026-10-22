/*
 * A341142  Number of partitions of n into 5 distinct prime powers (including
 *          1).
 * https://oeis.org/A341142
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 5 distinct prime powers
 *            (including 1)
 * Range:     u8 n <= 63, u16 n <= 504, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses a subset‑sum DP.  Results are cached so calls
   in any order remain fast.  It works for any seq_t/idx_t supplied in the
   build.  Only standard library facilities are used. */
#include <stdlib.h>
#include <stdbool.h>

static size_t max_n = 0;                 /* max n already processed      */
static seq_t **dp = NULL;                /* dp[k][s]  0 <= k <= 5        */

/* pragma: pack(8) unnecessary for seq_t as build decides           */
static void rebuild_dp(idx_t n_needed) {
    size_t new_max = (size_t)n_needed;

    /* free old tables */
    if (dp) {
        for (int k = 0; k <= 5; ++k) {
            if (dp[k]) free(dp[k]);
        }
        free(dp);
    }

    /* allocate new tables */
    dp = (seq_t**)malloc(6 * sizeof(seq_t*));
    if (!dp) return;               /* allocation failure – caller will return 0 */
    for (int k = 0; k <= 5; ++k) {
        dp[k] = (seq_t*)calloc(new_max + 1, sizeof(seq_t));
        if (!dp[k]) {
            for (int j = 0; j < k; ++j) free(dp[j]);
            free(dp);
            dp = NULL;
            return;
        }
    }

    /* ---- primes up to new_max ---- */
    bool *comp = (bool*)malloc((new_max + 1) * sizeof(bool));
    if (!comp) return;
    for (size_t i = 0; i <= new_max; ++i) comp[i] = false;
    for (size_t i = 2; i * i <= new_max; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= new_max; j += i)
                comp[j] = true;

    size_t prime_cnt = 0;
    for (size_t i = 2; i <= new_max; ++i) if (!comp[i]) ++prime_cnt;
    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) { free(comp); return; }
    size_t pc = 0;
    for (size_t i = 2; i <= new_max; ++i) if (!comp[i]) primes[pc++] = i;
    free(comp);

    /* ---- prime powers (including 1) ---- */
    /* estimate number of values: one for each prime plus its powers */
    size_t pp_est = prime_cnt + 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        while (val <= new_max / p) { /* next power fits */
            ++pp_est;
            val *= p;
        }
    }
    uint64_t *powers = (uint64_t*)malloc(pp_est * sizeof(uint64_t));
    if (!powers) { free(primes); return; }
    size_t pp_cnt = 0;
    powers[pp_cnt++] = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        while (val <= new_max) {
            powers[pp_cnt++] = val;
            if (val > new_max / p) break;
            val *= p;
        }
    }
    free(primes);

    /* ---- DP initialise ---- */
    dp[0][0] = 1;  /* one way to sum to 0 using 0 items */

    /* ---- DP over prime powers ---- */
    for (size_t e = 0; e < pp_cnt; ++e) {
        size_t v = (size_t)powers[e];
        for (int k = 5; k >= 1; --k) {
            for (size_t s = v; s <= new_max; ++s) {
                dp[k][s] += dp[k-1][s - v];
            }
        }
    }

    free(powers);
    max_n = new_max;
}

seq_t a(idx_t n) {
    if (n < 15) return 0;   /* offset safety – lower indices are irrelevant */

    if (!dp || (size_t)n > max_n) rebuild_dp(n);
    if (!dp) return 0;  /* allocation failed */

    return dp[5][(size_t)n];
}
