/*
 * A345492  Numbers that are the sum of eight squares in five or more ways.
 * https://oeis.org/A345492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 204, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345492
 * Numbers that are the sum of eight (positive) squares in five or more ways.
 * The function below builds the list of such numbers by enumerating
 * non‑decreasing 8‑tuples of square values (1²,2²,3²,…) with a
 * simple DP that keeps the count of representations capped at five.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* maximum number to pre‑compute; large enough for 10 000 terms */
#define MAX_SUM 200000

/* DP table: dp[k][n] = number of ways (capped at 5) to write n as
 * the sum of k positive squares, using squares up to the current one.
 * only 0 ≤ k ≤ 8, 0 ≤ n ≤ MAX_SUM.
 */
static unsigned char dp[9][MAX_SUM + 1];

/* array of terms found (1‑indexed when returned by a) */
static seq_t *good = NULL;
static size_t good_len = 0;
static bool built = false;

/* build the table and the list of terms once */
static void build(void)
{
    /* base case: 0 squares sum to 0 in exactly one way */
    for (int k = 0; k <= 8; ++k)
        for (int n = 0; n <= MAX_SUM; ++n)
            dp[k][n] = 0;
    dp[0][0] = 1;

    /* iterate over all positive square values */
    for (int i = 1; ; ++i) {
        int v = i * i;
        if (v > MAX_SUM) break;

        /* for each new square, update dp for all k and n (unbounded use) */
        for (int k = 1; k <= 8; ++k) {
            for (int n = v; n <= MAX_SUM; ++n) {
                unsigned char add = dp[k - 1][n - v];
                if (!add) continue;
                unsigned char cur = dp[k][n];
                unsigned char all = cur + add;
                dp[k][n] = all > 5 ? 5 : all;   /* cap at 5 */
            }
        }
    }

    /* collect numbers with at least 5 representations */
    /* allocate generous space; MAX_SUM is enough to hold more than 10 000 terms */
    good = (seq_t *)malloc(sizeof(seq_t) * (MAX_SUM + 1));
    if (!good) { exit(1); }

    good_len = 0;
    for (int n = 1; n <= MAX_SUM; ++n) {
        if (dp[8][n] >= 5) {
            good[good_len++] = (seq_t)n;
        }
    }
}

/* ----- public function ----- */
seq_t a(idx_t n)
{
    if (!built) {
        build();
        built = true;
    }
    if (n == 0 || n > good_len) return 0;
    return good[n - 1];
}
