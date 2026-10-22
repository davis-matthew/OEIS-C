/*
 * A345772  Numbers that are the sum of six cubes in exactly ten ways.
 * https://oeis.org/A345772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer that can be written as the sum
 *            of six positive cubes in exactly ten unordered ways
 * Range:     u8 none, u16 n <= 1453, u32 n <= 1454, u64 n <= 1454,
 *            u128 n <= 1454, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_cube_root.h"

static seq_t *global_terms = NULL;   /* sorted list of terms found so far   */
static size_t global_terms_sz = 0;    /* number of stored terms               */
static idx_t  global_limit   = 0;     /* largest number that has been examined */

/* ------------------------------------------------------------------ */
/* Compute all terms less than or equal to 'limit'.  The result is an
   array of seq_t values in ascending order.  The caller is responsible
   for freeing the returned array. */
static seq_t *
compute_terms(idx_t limit, size_t *out_cnt)
{
    /* maximum cube root needed for the current limit */
    idx_t max_c = int_cube_root(limit);
    size_t max_limit = (size_t)limit;            /* for array bounds */

    /* dp[k][s] = number of ways to form 's' with exactly 'k' cubes
       from the cubes processed so far.  Counts are kept up to full
       integer width (uint16_t) because the numbers of combinations
       stay well below 65535 for any reasonable limit. */
    uint16_t **dp = (uint16_t **)malloc(7 * sizeof(uint16_t *));
    if (!dp) return NULL;
    for (int k = 0; k < 7; ++k) {
        dp[k] = (uint16_t *)calloc(max_limit + 1, sizeof(uint16_t));
        if (!dp[k]) {
            for (int t = 0; t < k; ++t) free(dp[t]);
            free(dp);
            return NULL;
        }
    }
    dp[0][0] = 1;          /* base: zero cubes sum to 0 */

    /* iterate over cube values, updating dp for each]
       using the standard coin–change pattern. */
    for (idx_t c = 1; c <= max_c; ++c) {
        idx_t cube = c * c * c;
        size_t v = (size_t)cube;
        for (int k = 1; k <= 6; ++k) {
            uint16_t *prev = dp[k - 1];
            uint16_t *cur  = dp[k];
            for (size_t s = v; s <= max_limit; ++s) {
                cur[s] += prev[s - v];
            }
        }
    }

    /* collect the sums having exactly ten representations */
    size_t cnt = 0;
    for (size_t s = 0; s <= max_limit; ++s)
        if (dp[6][s] == 10) ++cnt;

    seq_t *res = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (res) {
        size_t idx = 0;
        for (size_t s = 0; s <= max_limit; ++s)
            if (dp[6][s] == 10) res[idx++] = (seq_t)s;
    }

    /* free dp arrays */
    for (int k = 0; k < 7; ++k) free(dp[k]);
    free(dp);

    *out_cnt = cnt;
    return res;   /* may be NULL on allocation failure */
}

/* ------------------------------------------------------------------ */
/* Extend the global term cache until it contains at least 'need' terms.
   The cache is rebuilt from scratch each time it is enlarged. */
static void
ensure_terms(size_t need)
{
    if (global_terms_sz >= need) return;

    /* initial guess; may be increased if not enough terms are found. */
    idx_t limit = global_limit ? global_limit : 250000;
    while (1) {
        size_t cnt = 0;
        seq_t *new_terms = compute_terms(limit, &cnt);
        if (!new_terms) break;   /* allocation failed – abort */

        if (cnt >= need) {
            free(global_terms);
            global_terms = new_terms;
            global_terms_sz = cnt;
            global_limit   = limit;
            return;
        }

        free(new_terms);   /* not enough – increase the range */
        limit *= 2;
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point – returns the n‑th (1‑based) term of the
   sequence.  For n == 0 the function returns 0 as a safety value. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0 */
    ensure_terms((size_t)n);
    return global_terms[n - 1];        /* offset 1 */
}
