/*
 * A344245  Number of partitions of n into 5 semiprime parts.
 * https://oeis.org/A344245
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #partitions of n into 5 semiprime parts (unordered)
 * Range:     u8 n <= 99, u16 n <= 460, u32 n <= 9160, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Assume seq_t and idx_t are provided by the build environment. */
static seq_t *cache = NULL;      /* stores a(i) for i<=max_n */
static size_t cache_len = 0;     /* cache capacity */
static idx_t max_n = 0;          /* maximum n computed so far */

/* forward declaration */
static void recompute(idx_t new_n);

seq_t a(idx_t n) {
    if (n < 20) return (seq_t)0; /* undefined below offset; return 0 */
    if (n > max_n) {
        recompute(n);
    }
    return cache[n];
}

/* recompute up to new_n (n >= max_n) */
static void recompute(idx_t new_n) {
    /* allocate semiprime list up to new_n */
    uint64_t *spf = NULL;
    size_t spf_cap = 0;
    /* build smallest prime factor table */
    {
        spf_cap = new_n + 1;
        spf = (uint64_t*)malloc(spf_cap * sizeof(uint64_t));
        if (!spf) exit(1);
        memset(spf, 0, spf_cap * sizeof(uint64_t));
        for (idx_t i = 2; i <= new_n; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if (i > new_n / i) continue;
                for (idx_t j = i * i; j <= new_n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    /* collect semiprimes */
    uint32_t *sems = NULL;
    size_t sems_cap = 0, sems_cnt = 0;
    for (idx_t i = 4; i <= new_n; ++i) {
        idx_t x = i, omega = 0;
        while (x > 1) { omega++; x /= spf[x]; }
        if (omega == 2) {
            if (sems_cnt == sems_cap) {
                sems_cap = sems_cap ? sems_cap << 1 : 64;
                sems = (uint32_t*)realloc(sems, sems_cap * sizeof(uint32_t));
                if (!sems) exit(1);
            }
            sems[sems_cnt++] = (uint32_t)i;
        }
    }
    free(spf);

    /* DP for partitions into exactly 5 parts */
    size_t stride = new_n + 1;
    seq_t *dp = (seq_t*)calloc(6 * stride, sizeof(seq_t));
    if (!dp) exit(1);
    dp[0 * stride + 0] = (seq_t)1;  /* 1 way to make 0 with 0 parts */

    for (size_t si = 0; si < sems_cnt; ++si) {
        uint32_t p = sems[si];
        for (idx_t k = 1; k <= 5; ++k) {
            idx_t off_k = k * stride;
            idx_t off_km1 = (k - 1) * stride;
            for (idx_t s = p; s <= new_n; ++s) {
                dp[off_k + s] += dp[off_km1 + s - p];
            }
        }
    }

    /* extend cache */
    if (cache_len <= new_n) {
        seq_t *new_cache = (seq_t*)realloc(cache, (new_n + 1) * sizeof(seq_t));
        if (!new_cache) exit(1);
        memset(new_cache + cache_len, 0, (new_n + 1 - cache_len) * sizeof(seq_t));
        cache = new_cache;
        cache_len = new_n + 1;
    }
    /* copy results for partition counts */
    for (idx_t i = 0; i <= new_n; ++i) {
        cache[i] = dp[5 * stride + i];
    }

    /* cleanup */
    free(sems);
    free(dp);

    max_n = new_n;
}
