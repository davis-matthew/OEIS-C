/*
 * A345495  Numbers that are the sum of eight squares in eight or more ways.
 * https://oeis.org/A345495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the numbers that can be expressed as a sum of
   eight positive squares in at least eight distinct unordered ways.
   The function memoises the sequence up to the requested index. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *ways[9]    = {0};          /* ways[k][s] = #ways to form s with exactly k positive squares */
static size_t maxSum     = 0;            /* current DP bound */
static seq_t *repCount   = NULL;         /* repCount[s] = number of ways to form s with 8 squares */
static seq_t *terms      = NULL;         /* list of qualifying numbers, sorted */
static size_t termsLen   = 0;            /* number of elements in terms */

/* Rebuild tables so that at least 'need' terms are available. */
static void extend_precomp(size_t need)
{
    while (termsLen < need) {
        /* increase the search bound */
        size_t newMax = (maxSum == 0) ? 64 : maxSum * 2;

        /* free old tables */
        for (int k = 0; k <= 8; ++k) {
            free(ways[k]);
            ways[k] = NULL;
        }
        free(repCount);
        repCount = NULL;
        free(terms);
        terms = NULL;
        termsLen = 0;

        /* allocate new DP tables */
        for (int k = 0; k <= 8; ++k) {
            ways[k] = (seq_t *)calloc(newMax + 1, sizeof(seq_t));
        }
        repCount = (seq_t *)calloc(newMax + 1, sizeof(seq_t));

        /* initialise DP: zero squares give sum 0 only */
        ways[0][0] = 1;

        /* DP over all positive squares <= newMax */
        for (idx_t i = 1; (size_t)(i * i) <= newMax; ++i) {
            idx_t sq = i * i;              /* current square value */
            for (int k = 1; k <= 8; ++k) {
                for (size_t s = sq; s <= newMax; ++s) {
                    seq_t add = ways[k-1][s - sq];
                    if (!add) continue;
                    seq_t val = ways[k][s] + add;
                    if (val > 8) val = 8;   /* saturation, we only need the threshold */
                    ways[k][s] = val;
                }
            }
        }

        /* compute number of 8‑square representations for each sum */
        for (size_t s = 1; s <= newMax; ++s) {
            repCount[s] = ways[8][s];
        }

        /* rebuild the list of qualifying numbers */
        size_t newLen = 0;
        for (size_t s = 1; s <= newMax; ++s) {
            if (repCount[s] >= 8) ++newLen;
        }
        seq_t *newTerms = (seq_t *)calloc(newLen, sizeof(seq_t));
        size_t idx = 0;
        for (size_t s = 1; s <= newMax; ++s) {
            if (repCount[s] >= 8) {
                newTerms[idx++] = (seq_t)s;
            }
        }

        terms = newTerms;
        termsLen = newLen;
        maxSum = newMax;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined before offset 1 */
    extend_precomp((size_t)n);
    return terms[n - 1];
}
