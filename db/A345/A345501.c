/*
 * A345501  Numbers that are the sum of nine squares in four or more ways.
 * https://oeis.org/A345501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A345501: Numbers that are the sum of nine positive squares in
    four or more distinct unordered ways. The implementation uses a
    dynamic‑programming sieve over square values up to MAX_SUM.  For each
    square value s = j^2 (j ≥ 1) the DP array dp[k][m] keeps the number
    (capped at 4 for efficiency) of ways to write the integer m as a
    sum of exactly k squares.  When all squares have been processed,
    all m with dp[9][m] ≥ 4 are stored in a static cache; a(idx) picks
    the (n‑1)th element of that cache.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SUM 20000           /* must be large enough to contain ≥10 000 terms */

static bool initialized = false;
static seq_t *cache = NULL;    /* array of sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/*  dp_current[k][m] holds the number of ways (capped at 4) to represent
    m as a sum of exactly k squares using only processed square values. */
static uint8_t dp_current[10][MAX_SUM + 1];
static uint8_t dp_next[10][MAX_SUM + 1];

/*  Resize the cache array, doubling capacity as needed.  */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation error – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/*  Populate cache with all n such that n can be written as a sum of
    nine positive squares in at least four unordered distinct ways.   */
static void compute_sequence(void)
{
    /* initialise DP */
    memset(dp_current, 0, sizeof(dp_current));
    dp_current[0][0] = 1;        /* one way to write 0 as sum of 0 squares */

    /* process each square value s = j^2, j ≥ 1 */
    for (int j = 1; j * j <= MAX_SUM; ++j) {
        int s = j * j;

        /* dp_next starts as dp_current (zero copies of this square) */
        memcpy(dp_next, dp_current, sizeof(dp_current));

        for (int k = 0; k < 9; ++k) {
            for (int m = 0; m <= MAX_SUM - s; ++m) {
                uint8_t val = dp_current[k][m];
                if (!val) continue;
                int max_copies = 9 - k;
                int max_by_sum = (MAX_SUM - m) / s;
                if (max_by_sum < 1) continue;
                if (max_by_sum < max_copies) max_copies = max_by_sum;
                for (int copies = 1; copies <= max_copies; ++copies) {
                    int new_k = k + copies;
                    int new_m = m + copies * s;
                    uint8_t new_val = dp_next[new_k][new_m] + val;
                    if (new_val > 4) new_val = 4;
                    dp_next[new_k][new_m] = new_val;
                }
            }
        }

        /* previous dp becomes the new current dp for the next square */
        memcpy(dp_current, dp_next, sizeof(dp_current));
    }

    /* collect all integers with at least four unordered representations */
    for (int n = 0; n <= MAX_SUM; ++n) {
        if (dp_current[9][n] >= 4) {
            grow_cache();
            cache[cache_len++] = (seq_t)n;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_sequence();
    if (n == 0) return 0; /* undefined for n < 1 – safe guard */
    if ((idx_t)(n - 1) >= cache_len) return 0; /* out of pre‑computed range */
    return cache[(size_t)(n - 1)];
}
