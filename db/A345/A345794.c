/*
 * A345794  Numbers that are the sum of nine cubes in exactly two ways.
 * https://oeis.org/A345794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 105, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all sums of nine cubes and count
   the number of distinct representations.
   A number occurs in the sequence iff it has exactly two representations
   as a sum of nine positive cubes (order of the cubes does not matter). */
#include <stdlib.h>
#include <stdint.h>

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t current_max_sum = 0;

/* Recompute the list of terms up to a given maximum sum. */
static void recompute_terms(size_t max_sum)
{
    /* free previous list */
    free(terms);
    terms = NULL;
    terms_len = 0;

    /* special case: nothing to compute */
    if (max_sum == 0)
        return;

    /* dp[sum][k] stored as linear array, 10 columns for k=0..9 */
    const size_t COLS = 10;
    size_t dp_size = (max_sum + 1) * COLS;
    uint8_t *dp = (uint8_t *)calloc(dp_size, sizeof(uint8_t));
    if (!dp)
        return;                 /* out of memory – terminate */

    /* base case: 0 cubes sum to 0 */
    dp[0] = 1;

    /* generate all cube values up to max_sum (exclude 0^3) */
    for (seq_t a = 1;; ++a) {
        seq_t c = a * a * a;
        if (c > max_sum)
            break;
        size_t cube = (size_t)c;

        for (size_t k = 1; k <= 9; ++k) {
            for (size_t s = cube; s <= max_sum; ++s) {
                uint8_t prev = dp[(s - cube) * COLS + (k - 1)];
                if (!prev)
                    continue;
                uint8_t cur = dp[s * COLS + k] + prev;
                if (cur > 3)   /* we only care if count >= 3 */
                    cur = 3;
                dp[s * COLS + k] = cur;
            }
        }
    }

    /* count how many sums have exactly two representations */
    size_t count = 0;
    for (size_t s = 0; s <= max_sum; ++s) {
        if (dp[s * COLS + 9] == 2)
            ++count;
    }

    if (count == 0) {
        free(dp);
        return;
    }

    /* allocate storage for the terms */
    terms = (seq_t *)malloc(count * sizeof(seq_t));
    if (!terms) {        /* out of memory */
        free(dp);
        return;
    }

    /* fill the terms array */
    size_t pos = 0;
    for (size_t s = 0; s <= max_sum; ++s) {
        if (dp[s * COLS + 9] == 2)
            terms[pos++] = (seq_t)s;
    }

    terms_len = count;
    free(dp);
    current_max_sum = max_sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but keep safe */

    /* ensure we have computed enough terms */
    if (terms_len < (size_t)n) {
        /* expand the maximum sum until we have at least n terms
           or the bound becomes absurdly large                */
        size_t bound = current_max_sum;
        if (bound == 0)
            bound = 2000;          /* start with a reasonable bound   */

        while (terms_len < (size_t)n) {
            recompute_terms(bound);
            if (terms_len >= (size_t)n)
                break;
            /* double the bound for next iteration */
            bound *= 2;
            /* safety guard: if bound exceeds 10 million, stop */
            if (bound > 10000000)
                break;
        }
    }

    /* if we still don't have enough terms, return 0 */
    if (terms_len < (size_t)n)
        return (seq_t)0;

    return terms[n - 1];
}
