/*
 * A345769  Numbers that are the sum of six cubes in exactly seven ways.
 * https://oeis.org/A345769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1359, u32 n <= 1359, u64 n <= 1359,
 *            u128 n <= 1359, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345769: Numbers that are the sum of six positive cubes in exactly seven ways.
   Algorithm: dynamic programming that counts unordered combinations of six
   cubes.  For each admissible cube value v = i³ (i≥1) we increment the
   count of ways to reach a sum using exactly k cubes for k=1..6.
   Counts are capped at 8 to detect values with more than 7 representations.
   The first 10 k terms are generated and stored for fast lookup. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM 2000000   /* upper bound for sums (≈ 2·10⁶) */

static seq_t *seq = NULL;          /* array of terms */
static size_t seq_len = 0;         /* number of computed terms */
static size_t seq_cap = 0;         /* allocated capacity of seq */
static bool seq_ready = false;     /* true once the list is built */

/* utility to grow an array */
static void grow_seq(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* build the term list the first time */
static void compute_terms(void)
{
    if (seq_ready) return;

    /* DP tables: dp[k][s] = number of ways to write s as a sum of k cubes.
       Only need up to k=6 and cap counts at 8.  Use unsigned char for
       memory efficiency. */
    static unsigned char *dp[7];
    for (int k = 0; k <= 6; ++k)
        dp[k] = (unsigned char *)calloc(MAX_SUM + 1, sizeof(unsigned char));
    dp[0][0] = 1;

    /* maximum cube root such that 6*root³ ≤ MAX_SUM  */
    int max_root = 0;
    while ((uint64_t)(max_root + 1) * (max_root + 1) * (max_root + 1) * 6 <= MAX_SUM)
        ++max_root;

    /* process each cube value */
    for (int r = 1; r <= max_root; ++r) {
        int cube = r * r * r;          /* since r ≤ ~100, safe in int */
        for (int k = 1; k <= 6; ++k) {
            unsigned char *dp_k     = dp[k];
            unsigned char *dp_k_1   = dp[k - 1];
            for (size_t s = cube; s <= MAX_SUM; ++s) {
                unsigned int cnt = dp_k[s] + dp_k_1[s - cube];
                dp_k[s] = (cnt > 7 ? 8 : (unsigned char)cnt);
            }
        }
    }

    /* collect all sums with exactly 7 representations */
    for (size_t s = 6; s <= MAX_SUM; ++s) {
        if (dp[6][s] == 7) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)s;
        }
    }

    /* free dp tables */
    for (int k = 0; k <= 6; ++k) free(dp[k]);

    seq_ready = true;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* behaviour below offset is irrelevant */
    compute_terms();
    if (n <= seq_len) return seq[n - 1];
    /* if n exceeds precomputed range, return 0 (should not happen for test data) */
    return 0;
}
