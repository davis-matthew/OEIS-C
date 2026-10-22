/*
 * A345599  Numbers that are the sum of ten fourth powers in six or more ways.
 * https://oeis.org/A345599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the n-th number that can be expressed as the
   sum of ten fourth powers in at least six distinct ways.
   The implementation uses dynamic programming to count, for every
   integer s ≤ LIMIT, how many multisets of exactly ten fourth powers
   sum to s.  Sums with a count ≥ 6 are stored in a static array,
   and subsequent calls simply index that array.  The limit is chosen
   to contain the first 10 000 terms (LIMIT = 1 000 000). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The build environment supplies definitions for seq_t and idx_t;
   they are NOT declared here.          */

/* Static data for memoisation */
static seq_t   *cache_vals = NULL;
static size_t  cache_len  = 0;
static bool    initialized = false;

/* --------------------------------------------------------------------- */
static void initialise(void)
{
    if (initialized)
        return;

    /* Limit for the sums to be counted.  This covers at least the first
       10 000 terms of the sequence. */
    const idx_t LIMIT = 1000000ULL;

    /* Compute the largest integer whose fourth power ≤ LIMIT. */
    idx_t max_root = 0;
    for (idx_t i = 1;; ++i) {
        idx_t v = (idx_t)i * (idx_t)i * (idx_t)i * (idx_t)i;
        if (v > LIMIT)
            break;
        max_root = i;
    }
    if (max_root == 0)
        max_root = 1;                     /* safety */

    /* Size of each row in the DP table */
    const size_t stride = (size_t)(LIMIT + 1);

    /* Allocate and zero the DP table: 11 rows (k = 0..10) */
    unsigned char *dp = (unsigned char *)calloc(11 * stride, 1);
    if (!dp)
        return;

    dp[0 * stride + 0] = 1;              /* one way to use 0 terms for sum 0 */

    /* Pre‑compute the fourth powers up to max_root. */
    seq_t *fourths = (seq_t *)malloc((max_root + 1) * sizeof(seq_t));
    if (!fourths) {
        free(dp);
        return;
    }
    for (idx_t i = 1; i <= max_root; ++i)
        fourths[i] = (seq_t)i * (seq_t)i * (seq_t)i * (seq_t)i;

    /* Dynamic programming over successive values of v = i^4. */
    for (idx_t vi = 1; vi <= max_root; ++vi) {
        seq_t v_seq = fourths[vi];
        idx_t v     = (idx_t)v_seq;          /* v fits in idx_t */

        for (int k = 1; k <= 10; ++k) {
            size_t basek  = (size_t)k  * stride;
            size_t basek1 = (size_t)(k-1) * stride;
            for (idx_t s = v; s <= LIMIT; ++s) {
                unsigned char prev = dp[basek1 + (size_t)(s - v)];
                if (!prev)
                    continue;
                unsigned char new_val = dp[basek + (size_t)s] + prev;
                if (new_val > 6)
                    new_val = 6;               /* cap at 6 */
                dp[basek + (size_t)s] = new_val;
            }
        }
    }

    /* Collect all sums with at least six representations. */
    size_t cap = 0;
    for (idx_t s = 0; s <= LIMIT; ++s) {
        unsigned char ways = dp[10 * stride + (size_t)s];
        if (ways >= 6) {
            if (cache_len >= cap) {
                size_t new_cap = cap == 0 ? 1024 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(fourths);
                    free(dp);
                    cache_vals = NULL;
                    cache_len  = 0;
                    return;
                }
                cache_vals = tmp;
                cap = new_cap;
            }
            cache_vals[cache_len++] = (seq_t)s;
        }
    }

    free(fourths);
    free(dp);
    initialized = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, return 0 */

    initialise();

    if ((idx_t)(n - 1) >= cache_len)
        return 0;                       /* n beyond precomputed range */

    return cache_vals[n - 1];
}
