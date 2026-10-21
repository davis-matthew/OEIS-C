/*
 * A295751  Numbers that have exactly ten representations as a sum of seven
 *          nonnegative squares.
 * https://oeis.org/A295751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n‑th integer that has exactly ten representations as a sum of seven non‑negative squares. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Opaque state for memoization */
static seq_t *seq_vals   = NULL;   /* array of sequence terms */
static idx_t seq_len     = 0;      /* number of terms found */
static int   initialized = 0;      /* one‑time initialization flag */

/* Highest value we will sieve – large enough to capture all known terms. */
#define MAX_N 10000

/* Small integer square root (floor).  Works for unsigned 64‑bit values. */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Build the sequence the first time it is needed. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = 1;

    /* Dynamic programming table: dp[sum][k] is the number of ways to form
       `sum` as a sum of exactly `k` non‑negative squares.  We use 64‑bit
       integers because the counts are small (≤ 10).  The table is static
       so it is zero‑initialised automatically. */
    static uint64_t dp[MAX_N + 1][8];
    dp[0][0] = 1;          /* one way to make 0 using 0 squares */

    /* Count representations using only positive squares (1², 2², …). */
    idx_t max_a = isqrt(MAX_N);
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t s = a * a;
        for (idx_t t = s; t <= MAX_N; ++t) {
            for (idx_t k = 1; k <= 7; ++k) {
                dp[t][k] += dp[t - s][k - 1];
            }
        }
    }

    /* Add zero squares – they change the number of squares but not the sum. */
    for (idx_t t = 0; t <= MAX_N; ++t) {
        for (idx_t k = 1; k <= 7; ++k) {
            dp[t][k] += dp[t][k - 1];
        }
    }

    /* Collect all n with exactly ten representations. */
    size_t cap = 32;
    seq_vals = seq_alloc(cap);
    seq_len = 0;

    for (idx_t n = 1; n <= MAX_N; ++n) {
        if (dp[n][7] == 10) {
            if (seq_len == cap) {
                size_t new_cap = cap << 1;
                seq_vals = seq_realloc(seq_vals, cap, new_cap);
                cap = new_cap;
            }
            seq_vals[seq_len++] = (seq_t)n;
        }
    }
}

/* External interface required by the build system.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0 || n > seq_len) return 0;   /* out of range */
    return seq_vals[n - 1];
}
