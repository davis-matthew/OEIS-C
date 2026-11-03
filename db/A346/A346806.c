/*
 * A346806  Numbers that are the sum of ten squares in seven or more ways.
 * https://oeis.org/A346806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9472 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A346806 – Numbers that are the sum of ten positive squares in
   seven or more distinct unordered representations.
   The code builds the list of qualifying numbers by dynamic programming
   over all squares up to the current search limit.  It expands the limit
   as needed until it has found at least n terms.
*/

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* dp[sum*11 + k] – number of ways to express `sum` as a sum of exactly `k`
   positive squares, counted as unordered multisets and capped at 7. */
static void generate_terms_up_to(idx_t limit,
                                 seq_t **terms, size_t *len)
{
    /* Convert limit to a size_t for array sizing (should fit). */
    if (limit > SIZE_MAX) limit = SIZE_MAX;
    size_t L = (size_t)limit;

    /* Allocate DP table: (L+1) sums × 11/k entries. */
    size_t dp_size = (L + 1) * 11ull;
    uint8_t *dp = (uint8_t *)calloc(dp_size, 1);
    if (!dp) {
        *terms = NULL;
        *len   = 0;
        return;
    }
    dp[0] = 1;  /* dp[0][0] = 1 */

    /* For every possible square value v^2 (v ≥ 1) up to the limit */
    for (size_t v = 1; v * v <= L; ++v) {
        size_t sq = v * v;
        /* Update DP from high sums to avoid using a square repeatedly in
           the same iteration (since we allow repetitions, we traverse sums
           from sq to L). */
        for (size_t sum = sq; sum <= L; ++sum) {
            size_t idx_sum  = sum * 11;
            size_t idx_prev = (sum - sq) * 11;
            /* For each possible number of squares k (1..10) */
            for (int k = 1; k <= 10; ++k) {
                uint8_t add = dp[idx_prev + (k - 1)];
                if (!add) continue;
                uint8_t *cur = &dp[idx_sum + k];
                uint8_t val = *cur + add;
                *cur = (val > 7) ? 7 : val;
            }
        }
    }

    /* Count how many sums have at least 7 representations with 10 squares. */
    size_t count = 0;
    for (size_t sum = 1; sum <= L; ++sum)
        if (dp[sum * 11 + 10] >= 7) ++count;

    seq_t *list = NULL;
    if (count) {
        list = mem_new(seq_t, count);
        if (list) {
            size_t idx = 0;
            for (size_t sum = 1; sum <= L; ++sum)
                if (dp[sum * 11 + 10] >= 7)
                    list[idx++] = (seq_t)sum;
        } else {
            count = 0;
        }
    }

    free(dp);
    *terms = list;
    *len   = count;
}

seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static seq_t *found      = NULL;     /* array of terms */
    static size_t found_len  = 0;        /* number of terms */
    static idx_t  current_lim = 0;       /* current search limit */
    static int    initialized  = 0;      /* flag for first run */

    /* If we don't yet have n terms, grow the search window. */
    if (!initialized || found_len < n) {
        idx_t new_lim = current_lim ? current_lim * 2 : 2000;

        /* Keep extending until we have enough terms. */
        while (found_len < n) {
            seq_t *terms = NULL;
            size_t len   = 0;
            generate_terms_up_to(new_lim, &terms, &len);

            /* Replace the old list with the new one. */
            if (found) mem_free(found);
            found      = terms;
            found_len  = len;
            current_lim = new_lim;

            if (found_len >= n) break;
            /* Prevent integer overflow on the next doubling. */
            if (new_lim > SIZE_MAX / 2) break;
            new_lim *= 2;
        }
        initialized = 1;
    }

    /* n is 1‑based: a(1) is the first term. */
    return found[n - 1];
}
