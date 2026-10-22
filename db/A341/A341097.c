/*
 * A341097  Number of partitions of n into 9 distinct squarefree parts.
 * https://oeis.org/A341097
 *
 * Offset:    58    (a(58) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 distinct squarefree
 *            parts * (OEIS A341097, offset 58). The routine builds a
 *            dynamic‑programming * table up to the requested n
 * Range:     u8 n <= 88, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   sieve_primes_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes_up_to.h"

/* ----------------------------------------------------------------------- */
/* Helper: integer square root (floor) for idx_t                           */
/* ----------------------------------------------------------------------- */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ----------------------------------------------------------------------- */
/* Simple sieve: returns list of all primes ≤ limit                       */

/* ----------------------------------------------------------------------- */
/* Global dynamic‑programming table for 0..K parts for sums 0..max_n        */
/* ----------------------------------------------------------------------- */
#define K 9 /* required number of distinct parts */
static seq_t *dp_mem = NULL;     /* 1‑D array: rows 0..K, cols 0..max_n */
static size_t dp_cols = 0;       /* current number of columns (max_n+1) */
static bool initialized = false;

/* ----------------------------------------------------------------------- */
/* Rebuild table up to given limit (inclusive)                            */
/* ----------------------------------------------------------------------- */
static void recompute(idx_t limit)
{
    /* 1. List all squarefree numbers ≤ limit */
    idx_t sqrt_lim = isqrt_idx(limit);
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes_up_to(sqrt_lim, &primes, &prime_cnt);

    uint8_t *squarefree = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!squarefree) {
        free(primes);
        return;
    }
    for (idx_t i = 1; i <= limit; ++i)
        squarefree[i] = 1;   /* assume squarefree */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t sq = p * p;
        if (sq > limit) break;
        for (idx_t m = sq; m <= limit; m += sq)
            squarefree[m] = 0;
    }

    idx_t sf_count = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (squarefree[i]) ++sf_count;

    idx_t *sf_list = NULL;
    if (sf_count > 0) {
        sf_list = (idx_t*)malloc(sf_count * sizeof(idx_t));
        if (sf_list) {
            size_t pos = 0;
            for (idx_t i = 1; i <= limit; ++i)
                if (squarefree[i])
                    sf_list[pos++] = i;
        } else
            sf_count = 0;
    }

    /* 2. Allocate new DP table */
    size_t rows = K + 1;
    size_t cols = (size_t)limit + 1;
    size_t sz = rows * cols;
    seq_t *new_dp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!new_dp) {
        free(sf_list);
        free(squarefree);
        free(primes);
        return;
    }
    for (size_t i = 0; i < sz; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;              /* dp[0][0] = 1 */

    /* 3. Populate DP: each squarefree number used at most once */
    for (idx_t idx = 0; idx < sf_count; ++idx) {
        idx_t val = sf_list[idx];
        for (int k = K; k >= 1; --k) {
            size_t row_off  = (size_t)k   * cols;
            size_t prev_off = (size_t)(k-1) * cols;
            for (idx_t s = limit; s >= val; --s) {
                uint64_t add = (uint64_t)new_dp[row_off + s] +
                               (uint64_t)new_dp[prev_off + (s - val)];
                new_dp[row_off + s] = (seq_t)add;
                if (s == val) break;   /* prevent underflow */
            }
        }
    }

    /* 4. Clean up */
    free(sf_list);
    free(squarefree);
    free(primes);

    /* 5. Replace old table */
    seq_t *old_dp = dp_mem;
    dp_mem = new_dp;
    dp_cols = (size_t)limit + 1;
    initialized = true;
    if (old_dp) free(old_dp);
}

/* ----------------------------------------------------------------------- */
/* Public function: a(n) – partitions into 9 distinct squarefree parts     */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 58) return 0;                  /* before offset */

    if (!initialized || dp_cols <= (size_t)n) {
        recompute(n);
    }
    size_t row_off = (size_t)K * dp_cols;
    return dp_mem[row_off + (size_t)n];
}
