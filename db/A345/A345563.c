/*
 * A345563  Numbers that are the sum of six fourth powers in six or more ways.
 * https://oeis.org/A345563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer that can be expressed as the
 *            sum of six fourth powers in at least six distinct ways
 * Range:     u8 none, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper that computes all terms up to a given maximum sum. */
static void
compute_terms(idx_t maxSum, seq_t **out, size_t *len)
{
    /* dp[t][s] holds the number of representations of s using exactly t terms,
       capped at 6.  t = 0..6; s = 0..maxSum. */
    unsigned char **dp = (unsigned char **)malloc(7 * sizeof(unsigned char *));
    for (idx_t t = 0; t <= 6; ++t) {
        dp[t] = (unsigned char *)calloc((size_t)(maxSum + 1), 1);
    }
    dp[0][0] = 1;

    /* Enumerate fourth powers and update dp. */
    for (idx_t v = 1; ; ++v) {
        idx_t v2 = v * v;          /* v^2 */
        idx_t v4 = v2 * v2;        /* v^4 */
        if (v4 > maxSum) break;

        for (idx_t t = 1; t <= 6; ++t) {
            for (idx_t s = v4; s <= maxSum; ++s) {
                unsigned char add = dp[t - 1][s - v4];
                if (add) {
                    unsigned char newVal = dp[t][s] + add;
                    if (newVal > 6) newVal = 6;
                    dp[t][s] = newVal;
                }
            }
        }
    }

    /* First pass: count how many sums have at least 6 representations. */
    size_t cnt = 0;
    for (idx_t s = 0; s <= maxSum; ++s) {
        if (dp[6][s] == 6) {
            ++cnt;
        }
    }

    /* Allocate result array and fill it. */
    seq_t *terms = (seq_t *)malloc(cnt * sizeof(seq_t));
    size_t pos = 0;
    for (idx_t s = 0; s <= maxSum; ++s) {
        if (dp[6][s] == 6) {
            terms[pos++] = (seq_t)s;
        }
    }
    *out = terms;
    *len = cnt;

    /* Free dp arrays. */
    for (idx_t t = 0; t <= 6; ++t) {
        free(dp[t]);
    }
    free(dp);
}

static seq_t *cached_terms = NULL;
static size_t cached_len = 0;
static idx_t cached_max_sum = 0;

static void
ensure_terms(idx_t wanted)
{
    if (cached_len >= wanted) {
        return;
    }
    idx_t maxSum = cached_max_sum ? (cached_max_sum * 2) : 1000000;
    while (1) {
        seq_t *terms;
        size_t len;
        compute_terms(maxSum, &terms, &len);
        free(cached_terms);
        cached_terms = terms;
        cached_len = len;
        cached_max_sum = maxSum;
        if (cached_len >= wanted || maxSum >= 4000000) {
            break;
        }
        maxSum <<= 1;  /* double the range and try again */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    ensure_terms(n);
    return cached_terms[n - 1];
}
