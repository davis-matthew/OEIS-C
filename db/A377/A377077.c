/*
 * A377077  G.f.: Sum_{k>=0} x^(7*k^2) / Product_{j=1..7*k-1} (1 - x^j).
 * https://oeis.org/A377077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, 7*k² ≤ n} p_{≤7k-1}(n-7k²) where p_{≤m}(r) is the
 *            number of partitions of r whose parts do not exceed m
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 193, u64 n <= 616,
 *            u128 n <= 2144, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Maximum k needed for n up to 10 000: floor(sqrt(10000/7)) = 37. */
#define MAX_K 40

/* Cache of partition tables: dp[k] holds the array for k‑th block.
   dp_size[k] is the allocated length (>=1) of that array. */
static seq_t *dp[MAX_K] = { NULL };
static size_t dp_size[MAX_K] = { 0 };

/* Ensure that dp[k] is computed for all residues 0…N (inclusive).
   If the existing table is too small, allocate a new one, free the old
   and recompute the values from scratch (sequential DP). */
static void ensure_dp(int k, size_t N)
{
    if (k <= 0 || k >= MAX_K) return;          /* safety check */
    if (dp_size[k] > N) return;                /* already large enough */

    size_t new_len = N + 1;
    seq_t *new_arr = (seq_t *)alloc_seq_array(new_len);
    /* Initialize all entries to 0; the first element (0) must be 1. */
    for (size_t i = 0; i < new_len; ++i) new_arr[i] = 0;
    new_arr[0] = 1;

    int max_part = 7 * k - 1;                  /* partition parts up to this value */
    for (int part = 1; part <= max_part; ++part) {
        for (size_t m = part; m <= N; ++m) {
            new_arr[m] += new_arr[m - part];
        }
    }

    /* Replace old array if it existed. */
    if (dp[k] != NULL) {
        free_seq_array(dp[k]);
    }
    dp[k] = new_arr;
    dp_size[k] = new_len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t total = 0;

    /* There is no contribution for n = 0 because the sum starts at k = 1. */
    if (n == 0) return 0;

    /* Find the largest k such that 7*k^2 ≤ n. */
    int k_max = 0;
    for (int k = 1; ; ++k) {
        idx_t kk = (idx_t)k * (idx_t)k * 7;
        if (kk > n) break;
        ++k_max;
    }

    /* Sum partitions for each admissible k. */
    for (int k = 1; k <= k_max; ++k) {
        idx_t kk = (idx_t)k * (idx_t)k * 7;          /* 7*k² */
        idx_t t  = n - kk;                           /* remaining weight to partition */

        /* Ensure DP table is ready up to t. */
        ensure_dp(k, (size_t)t);

        total += dp[k][(size_t)t];
    }

    return total;
}
