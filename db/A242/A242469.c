/*
 * A242469  Number of length n+2 0..6 arrays with no three equal elements in a
 *          row and new values 0..6 introduced in 0..6 order.
 * https://oeis.org/A242469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP computation of A242469.
 *
 * The sequence counts length-(n+2) arrays over the alphabet {0,…,6}
 * with no three equal consecutive elements and new values introduced
 * in the order 0,1,2,…
 * The algorithm maintains a finite-state automaton for the maximum value
 * seen so far, the last value, and the current run length (1 or 2).
 * Transitions correspond to either repeating the last value (if the
 * run length is 1), switching to a smaller value, or introducing the
 * next new value (if less than 6).  The automaton is stepped until the
 * desired array length n+2 is reached; the sum of all state counts is
 * a(n).  The values are memoised so that repeated or out-of-order
 * queries are answered in O(1) after the initial computation.
 *
 * This implementation does not hard‑code any terms or use the
 * empirical recurrence; it simply follows the definition.
 */

#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Global state for memoisation and DP                                         */
/* --------------------------------------------------------------------- */
static seq_t dp[7][7][3];      /* current DP table */
static seq_t ndp[7][7][3];     /* work table for next step */
static size_t  current_len = 0;/* number of positions processed (>=1) */
static size_t  cache_len    = 0;/* number of terms stored (a(1) .. a(cache_len)) */
static size_t  cache_cap    = 0;/* capacity allocated for cache */
static seq_t * cache         = NULL;/* array of computed a(n) values */

/* --------------------------------------------------------------------- */
/* Advance the DP by one position                                           */
/* --------------------------------------------------------------------- */
static void dp_step(void)
{
    /* zero the next table */
    memset(ndp, 0, sizeof(ndp));

    /* perform all state transitions */
    for (int m = 0; m <= 6; ++m) {
        for (int l = 0; l <= 6; ++l) {
            for (int r = 1; r <= 2; ++r) {
                seq_t val = dp[m][l][r];
                if (!val) continue;

                /* repeat the last value if run length < 2 */
                if (r < 2) ndp[m][l][r+1] += val;

                /* choose any earlier value (≠ last) */
                for (int v = 0; v <= m; ++v) {
                    if (v == l) continue;
                    ndp[m][v][1] += val;
                }

                /* introduce the next new value, if allowed */
                if (m < 6) ndp[m+1][m+1][1] += val;
            }
        }
    }

    /* copy next table back to dp */
    memcpy(dp, ndp, sizeof(dp));
    ++current_len;
}

/* --------------------------------------------------------------------- */
/* Public function: returns a(n) for n ≥ 1                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 (offset 1).  Calls with n = 0
     * are not specified; for safety we return 0. */
    if (n == 0) return 0;

    /* One‑time initialisation of the DP for an empty array.  The first
     * element must be 0, so we initialise the state after processing
     * the first position. */
    if (current_len == 0) {
        memset(dp, 0, sizeof(dp));
        dp[0][0][1] = (seq_t)1;   /* array of length 1: [0] */
        current_len = 1;          /* one position processed */
    }

    /* Compute terms until the requested n is available in the cache. */
    while (cache_len < (size_t)n) {
        /* Target length for a(cache_len+1) is (cache_len+1)+2 */
        size_t target_len = (cache_len + 1) + 2;
        while (current_len < target_len)
            dp_step();

        /* Sum all DP states to obtain a(cache_len+1) */
        seq_t sum = 0;
        for (int m = 0; m <= 6; ++m)
            for (int l = 0; l <= 6; ++l)
                for (int r = 1; r <= 2; ++r)
                    sum += dp[m][l][r];

        /* Store the computed value in the cache. */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = sum;
        ++cache_len;
    }

    return cache[(size_t)n - 1];
}
