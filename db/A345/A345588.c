/*
 * A345588  Numbers that are the sum of nine fourth powers in four or more
 *          ways.
 * https://oeis.org/A345588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A345588: numbers that are the sum of nine fourth powers in
 * four or more unordered ways.
 *
 * Algorithm:
 *   1. Enumerate all fourth powers up to a cutoff LIMIT.
 *   2. Use dynamic programming to count, for every integer ≤ LIMIT,
 *      how many unordered multisets of 9 fourth powers sum to that integer.
 *      The DP table stores only the count capped at 4 to save space.
 *   3. Collect all integers whose count ≥ 4; the resulting list is sorted
 *      increasingly because we traverse the sums in order.
 *   4. On demand, increase LIMIT and rebuild the table until the
 *      requested n-th term is available.
 *
 * The routine uses only integer arithmetic on seq_t and size_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* forward declaration - types are supplied by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* internal state */

static seq_t *seq_terms = NULL;   /* table of terms */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t last_limit = 0;     /* last LIMIT used for construction */

/* ------------------------------------------------------------------ */
/* build the sequence up to a given integer LIMIT (inclusive)          */
static void build_sequence(size_t LIMIT)
{
    /* 1. list all fourth powers ≤ LIMIT */
    size_t i = 1;
    while (1) {
        uint64_t v = (uint64_t)i * i * i * i;
        if (v > LIMIT)
            break;
        ++i;
    }
    size_t num_coins = i - 1;
    size_t *coins = (size_t*)malloc(num_coins * sizeof(size_t));
    for (size_t j = 0; j < num_coins; ++j) {
        uint64_t vv = (uint64_t)(j + 1) * (j + 1) * (j + 1) * (j + 1);
        coins[j] = vv;
    }

    /* 2. DP table: dp[k][s] = #un #ways to write s as sum of k fourth powers,
     *    capped at 4 (so we only keep whether there are 0–4 ways). */
    const size_t MAX_K = 9;
    size_t sz = LIMIT + 1;
    uint8_t **dp = (uint8_t**)malloc((MAX_K + 1) * sizeof(uint8_t*));
    for (size_t k = 0; k <= MAX_K; ++k)
        dp[k] = (uint8_t*)calloc(sz, sizeof(uint8_t));

    dp[0][0] = 1;                      /* one way to make 0 with 0 parts */

    for (size_t ci = 0; ci < num_coins; ++ci) {
        size_t coin = coins[ci];
        for (size_t k = 1; k <= MAX_K; ++k) {
            for (size_t v = coin; v < sz; ++v) {
                uint8_t inc = dp[k - 1][v - coin];
                if (inc) {
                    uint8_t val = dp[k][v] + inc;
                    if (val > 4)
                        val = 4;
                    dp[k][v] = val;
                }
            }
        }
    }

    /* 3. collect sums with at least four representations */
    size_t cap = 1024;
    seq_terms = (seq_t*)malloc(cap * sizeof(seq_t));
    seq_len   = 0;
    for (size_t v = 1; v < sz; ++v) {
        if (dp[MAX_K][v] >= 4) {
            if (seq_len == cap) {
                cap <<= 1;
                seq_terms = (seq_t*)realloc(seq_terms, cap * sizeof(seq_t));
            }
            seq_terms[seq_len++] = (seq_t)v;
        }
    }

    /* 4. free temporaries */
    for (size_t k = 0; k <= MAX_K; ++k)
        free(dp[k]);
    free(dp);
    free(coins);
}

/* ------------------------------------------------------------------ */
/* public entry point */

/* Returns the n-th term (1‑based) of A345588.  The function is
 * memoised – the first call builds a table for an initial LIMIT,
 * subsequent calls may rebuild with a larger LIMIT if the requested n
 * was not yet available. */
seq_t a(idx_t n)
{
    /* sanity check */
    if (n == 0)
        return 0;               /* undefined, but harmless */

    /* if we haven't built yet, start with a modest LIMIT */
    if (seq_terms == NULL) {
        last_limit = 1200000;   /* heuristic limit that usually covers many terms */
        build_sequence(last_limit);
    }

    /* if the current table is too short, grow it until we have n terms
     * or until a hard limit is reached (to avoid infinite loops). */
    while (n > seq_len) {
        /* increase limit by roughly a factor of 1.5; abort if too large */
        if (last_limit > 8000000)   /* generous upper bound for typical n ≤ 10000 */
            break;
        last_limit = last_limit + last_limit / 2;
        /* free old table */
        free(seq_terms);
        seq_terms = NULL;
        seq_len   = 0;
        /* rebuild with bigger limit */
        build_sequence(last_limit);
    }

    if (n > seq_len)   /* impossible to find enough terms */
        return 0;

    return seq_terms[n - 1];
}
