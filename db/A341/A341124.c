/*
 * A341124  Number of partitions of n into 6 prime powers (including 1).
 * https://oeis.org/A341124
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 prime powers (including
 *            1) (the classical coin‑change DP with coins = {1} ∪ {p^k ≤ n | p
 *            prime, k≥1}). The DP counts unordered partitions (order of parts
 *            ignored)
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external integer types provided by the build system */
extern seq_t a(idx_t n);

/* comparison helper for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* simple sieve: generate all primes ≤ limit */
static void generate_primes_upto(idx_t limit, uint64_t **out, size_t *cnt_out)
{
    if (limit < 2) {
        *out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t cap = 64;
    uint64_t *primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t count = 0;
    bool *is_comp = (bool *)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) is_comp[i] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (idx_t m = p * p; m <= limit; m += p) is_comp[m] = true;
        }
    }
    for (idx_t n = 2; n <= limit; ++n) {
        if (!is_comp[n]) {
            if (count == cap) {
                cap <<= 1;
                primes = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
            }
            primes[count++] = n;
        }
    }
    free(is_comp);
    *out = primes;
    *cnt_out = count;
}

/* collect all prime powers (including 1) ≤ N */
static void generate_prime_powers(idx_t N, uint64_t **out, size_t *cnt_out)
{
    size_t cap = 64;
    uint64_t *powers = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t count = 0;
    powers[count++] = 1;                    /* 1 is a valid prime power */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    generate_primes_upto(N, &primes, &prime_cnt);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t v = p;
        while (v <= (uint64_t)N) {
            if (count == cap) {
                cap <<= 1;
                powers = (uint64_t *)realloc(powers, cap * sizeof(uint64_t));
            }
            powers[count++] = v;
            if (v > (uint64_t)N / p) break; /* avoid overflow */
            v *= p;
        }
    }
    free(primes);
    qsort(powers, count, sizeof(uint64_t), cmp_uint64);
    *out = powers;
    *cnt_out = count;
}

/* static storage for memoisation */
static seq_t *dpbuf = NULL;        /* contiguous DP table */
static idx_t  cache_n = 0;         /* highest n that has been pre‑computed */
static idx_t  stride = 0;          /* stride = cache_n + 1 */

/* rebuild the DP table up to N if needed */
static void recompute(idx_t N)
{
    if (N <= cache_n) return;
    free(dpbuf);
    stride = N + 1;
    size_t total = (size_t)stride * 7;            /* parts 0..6 */
    dpbuf = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dpbuf) exit(1);                       /* fatal */
    dpbuf[0] = 1;                               /* 0 sum with 0 parts */

    /* generate all prime powers up to N */
    uint64_t *powers = NULL;
    size_t pow_cnt = 0;
    generate_prime_powers(N, &powers, &pow_cnt);

    /* DP: for each coin, update partitions with 1..6 parts */
    for (size_t pi = 0; pi < pow_cnt; ++pi) {
        idx_t v = (idx_t)powers[pi];
        for (idx_t parts = 1; parts <= 6; ++parts) {
            seq_t *dp_curr  = dpbuf + parts * stride;
            seq_t *dp_prev  = dpbuf + (parts - 1) * stride;
            for (idx_t sum = v; sum <= N; ++sum) {
                dp_curr[sum] += dp_prev[sum - v];
            }
        }
    }
    free(powers);
    cache_n = N;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 6) return 0;                 /* behaviour for n < offset is irrelevant */
    recompute(n);                        /* ensure table computed up to n */
    return dpbuf[6 * stride + n];        /* dp[6][n] */
}
