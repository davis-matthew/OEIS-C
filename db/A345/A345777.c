/*
 * A345777  Numbers that are the sum of seven cubes in exactly five ways.
 * https://oeis.org/A345777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 351, u32 n <= 351, u64 n <= 351, u128 n <= 351,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A345777
 * Numbers that are the sum of seven cubes in exactly five ways.
 *
 * The function computes the nth term (n ≥ 1) by dynamic programming.
 * For all cubes c = k³ (k ≥ 1) we count unordered 7‑tuples
 * of such cubes whose sum equals s.
 *
 * DP state:  counts[sum][k]  – number of unordered sums equal to `sum`
 *            using exactly `k` cubes.
 *  DP transition (for each cube `c`):
 *      for sum  from c to LIMIT
 *          for k = 1 … 7
 *              counts[sum][k] += counts[sum - c][k - 1]
 *              (capped at 6 to keep values small).
 *  After processing all cubes, counts[s][7] holds the number of
 *  unordered representations of `s` as the sum of seven cubes.
 *  Values with count == 5 are stored as sequence terms.
 *
 * The function caches results and expands the search limit
 * by doubling until at least the requested `n` terms are found.
 *
 * This implementation is valid C11 and compiles as C++17 as well.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- Global state for memoisation --------------------------------------------------- */
static idx_t *terms = NULL;      /* array of found terms (ascending order) */
static size_t terms_len = 0;     /* number of terms currently stored */
static size_t terms_cap = 0;     /* allocation capacity for `terms`  */
static idx_t  current_limit = 0;/* maximum integer value considered */

/* --- Helper: recompute all terms up to `new_limit` -------------------------------- */
static void recompute(idx_t new_limit)
{
    /* Allocate DP array:  (new_limit+1) sums × 8 (k=0..7) entries */
    size_t dp_len = (size_t)new_limit + 1;
    uint8_t *dp = (uint8_t *)calloc(dp_len * 8, 1); /* all zeros except dp[0][0] = 1 */
    if (!dp) return;                               /* allocation failure → abort */

    dp[0] = 1; /* sum 0 with 0 cubes */

    /* Process every cube value c = k³ (k ≥ 1) that satisfies c ≤ new_limit */
    for (idx_t k = 1;; ++k) {
        idx_t c = k * k * k;
        if (c > new_limit) break;

        /* DP transition for this cube */
        for (idx_t sum = c; sum <= new_limit; ++sum) {
            size_t cur_base  = (size_t)sum * 8;
            size_t prev_base = (size_t)(sum - c) * 8;
            for (int k_used = 1; k_used <= 7; ++k_used) {
                uint8_t add = dp[prev_base + (k_used - 1)];
                if (!add) continue;          /* no way to reach sum‑c with k_used‑1 cubes */
                uint8_t old = dp[cur_base + k_used];
                uint8_t nv  = old + add;
                if (nv > 5) nv = 6;          /* cap at 6 (≥5) to avoid overflow */
                dp[cur_base + k_used] = nv;
            }
        }
    }

    /* ------------------------------------------------------------------------------ */
    /* Collect all integers s with exactly 5 unordered representations.          */
    /* Free previously stored terms.                                              */
    free(terms);
    terms = NULL;
    terms_len = 0;
    terms_cap = 0;

    /* Reserve initial capacity for collected terms */
    terms_cap = 16;
    terms = (idx_t *)malloc(terms_cap * sizeof(idx_t));
    if (!terms) { free(dp); return; }

    for (idx_t s = 1; s <= new_limit; ++s) {
        if (dp[(size_t)s * 8 + 7] == 5) {
            if (terms_len >= terms_cap) {
                terms_cap <<= 1; /* double capacity */
                idx_t *tmp = (idx_t *)realloc(terms, terms_cap * sizeof(idx_t));
                if (!tmp) { free(dp); return; }
                terms = tmp;
            }
            terms[terms_len++] = s;
        }
    }

    free(dp);
    current_limit = new_limit;
}

/* --- Public interface -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined behaviour for n=0 */

    /* Expand precomputation until we have at least n terms. */
    while (terms_len < (size_t)n) {
        idx_t new_limit = (current_limit == 0) ? 1000 : current_limit * 2;
        recompute(new_limit);
    }

    return (seq_t)terms[n - 1];
}
