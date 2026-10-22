/*
 * A341136  Number of ways to write n as an ordered sum of 7 prime powers
 *          (including 1).
 * https://oeis.org/A341136
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via DP: number of compositions of n into 7 prime powers (including 1) */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- helpers for prime powers ---------- */
static idx_t *prime_powers = NULL;
static size_t pp_len = 0;
static size_t pp_cap = 0;
static idx_t prime_limit = 0;

/* compare for qsort */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t aa = *(const idx_t *)a;
    idx_t bb = *(const idx_t *)b;
    return (aa < bb) ? -1 : (aa > bb);
}

/* generate all prime powers <= limit (including 1), sort and deduplicate */
static void extend_prime_powers(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    size_t l = (size_t)limit + 1;
    bool *is_prime = (bool *)calloc(l, sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 2; i < l; ++i) is_prime[i] = true;
    for (size_t p = 2; p * p < l; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m < l; m += p)
                is_prime[m] = false;

    for (size_t p = 2; p < l; ++p) {
        if (!is_prime[p]) continue;
        idx_t val = (idx_t)p;
        while (val <= limit) {
            if (pp_len == pp_cap) {
                size_t new_cap = pp_cap ? pp_cap * 2 : 64;
                idx_t *tmp = (idx_t *)realloc(prime_powers, new_cap * sizeof(idx_t));
                if (!tmp) abort();
                prime_powers = tmp;
                pp_cap = new_cap;
            }
            prime_powers[pp_len++] = val;
            if (val > limit / (idx_t)p) break;
            val *= (idx_t)p;
        }
    }

    /* add 1 (always a prime power) */
    if (pp_len == 0 || prime_powers[0] != 1) {
        if (pp_len == pp_cap) {
            size_t new_cap = pp_cap ? pp_cap * 2 : 64;
            idx_t *tmp = (idx_t *)realloc(prime_powers, new_cap * sizeof(idx_t));
            if (!tmp) abort();
            prime_powers = tmp;
            pp_cap = new_cap;
        }
        prime_powers[pp_len++] = 1;
    }

    free(is_prime);

    qsort(prime_powers, pp_len, sizeof(idx_t), cmp_idx_t);
    size_t uniq = 0;
    for (size_t i = 0; i < pp_len; ++i)
        if (!uniq || prime_powers[uniq - 1] != prime_powers[i])
            prime_powers[uniq++] = prime_powers[i];
    pp_len = uniq;
    prime_limit = limit;
}

/* ---------- DP tables ---------- */
static seq_t *dp[8] = {0};
static idx_t dp_cap = 0;

/* ensure that dp tables are computed up to n */
static void ensure_up_to(idx_t n)
{
    if (n <= dp_cap)
        return;

    extend_prime_powers(n);

    /* resize each dp[j] to length n+1 */
    for (int j = 0; j <= 7; ++j) {
        seq_t *new_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_arr) abort();
        memset(new_arr, 0, (n + 1) * sizeof(seq_t));
        if (dp[j]) {
            memcpy(new_arr, dp[j], (dp_cap + 1) * sizeof(seq_t));
            free(dp[j]);
        }
        dp[j] = new_arr;
    }

    /* base case dp[0][0] = 1 already set during init */
    if (!dp[0]) {
        dp[0] = (seq_t *)calloc(1, sizeof(seq_t));
        if (!dp[0]) abort();
        dp[0][0] = (seq_t)1;
    }

    /* compute new entries */
    for (idx_t i = dp_cap + 1; i <= n; ++i) {
        for (int j = 1; j <= 7; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < pp_len && prime_powers[k] <= i; ++k) {
                idx_t idx = i - prime_powers[k];
                sum += dp[j - 1][(size_t)idx];
            }
            dp[j][(size_t)i] = sum;
        }
    }

    dp_cap = n;
}

#if !defined(INITIALIZED)
/* one-time static initialization at first call */
static bool initialized = false;
#endif

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;   /* undefined for n < 7, return 0 */

    if (!initialized) {
        /* allocate dp[0]..dp[7] fields */
        for (int j = 0; j <= 7; ++j) {
            dp[j] = (seq_t *)calloc(1, sizeof(seq_t));
            if (!dp[j]) abort();
        }
        dp[0][0] = (seq_t)1;   /* base case */
        initialized = true;
    }

    ensure_up_to(n);
    return dp[7][(size_t)n];
}
