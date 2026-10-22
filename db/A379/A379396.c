/*
 * A379396  Numbers that can be written in exactly one way as a sum of at most
 *          nine positive third powers.
 * https://oeis.org/A379396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379396: numbers that have exactly one representation as a sum of at most
   nine positive cubes.  The implementation uses dynamic programming to count the
   representations up to a growing limit and stores the indices that have count
   exactly one.  It is memoised so repeated calls are O(1). */

#include <stdlib.h>
#include <stdint.h>

/* static storage for memoisation */
static idx_t     *cand_idx = NULL;   /* indices of sought numbers      */
static size_t    cand_len  = 0;      /* number of elements stored     */
static size_t    cand_cap  = 0;      /* allocated capacity            */
static uint8_t   *dp_arr    = NULL;   /* DP array (size: (limit+1)*10) */
static idx_t    dp_limit  = 0;        /* current DP limit              */

/* Append a candidate index to the memoisation table. */
static void append_candidate(idx_t val)
{
    if (cand_len == cand_cap) {
        cand_cap = cand_cap ? cand_cap * 2 : 128;
        cand_idx = (idx_t *)realloc(cand_idx, cand_cap * sizeof(idx_t));
    }
    cand_idx[cand_len++] = val;
}

/* Recompute DP and candidate list up to a given LIMIT. */
static void recompute(idx_t LIMIT)
{
    /* free previous DP storage */
    free(dp_arr);

    /* allocate and zero DP array */
    size_t dp_size = (size_t)(LIMIT + 1) * 10;
    uint8_t *dp = (uint8_t *)calloc(dp_size, 1);
    if (!dp) exit(1); /* out of memory – terminate */

    /* base case: 0 can be made with 0 cubes */
    dp[0] = 1; /* dp[0][0] = 1 */

    /* iterate over all cubes ≤ LIMIT */
    for (idx_t c = 1;; ++c) {
        idx_t cube = c * c * c;
        if (cube > LIMIT) break;

        for (idx_t s = cube; s <= LIMIT; ++s) {
            size_t base      = (size_t)s * 10;
            size_t prev_base = (size_t)(s - cube) * 10;
            for (int k = 1; k <= 9; ++k) {
                uint8_t prev = dp[prev_base + (k - 1)];
                if (prev) {
                    uint8_t cur = dp[base + k];
                    uint8_t newv = cur + prev;
                    if (newv > 2) newv = 2; /* cap at 2 (≥2 representations) */
                    dp[base + k] = newv;
                }
            }
        }
    }

    /* free previous candidate storage & reset */
    free(cand_idx);
    cand_idx = NULL;
    cand_len = 0;
    cand_cap = 0;

    /* collect all indices with exactly one representation */
    /* include 0 explicitly – the sequence starts with a(1)=0 */
    append_candidate((idx_t)0);
    for (idx_t s = 1; s <= LIMIT; ++s) {
        uint8_t sum = 0;
        size_t base = (size_t)s * 10;
        for (int k = 1; k <= 9; ++k) {
            sum += dp[base + k];
            if (sum > 1) break; /* early exit if there are ≥2 ways */
        }
        if (sum == 1) {
            append_candidate(s);
        }
    }

    dp_arr   = dp;
    dp_limit = LIMIT;
}

/* Main sequence function.  External linkage, as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, but safe */

    /* Ensure we have at least a few terms precomputed. */
    if (dp_arr == NULL) {
        recompute(1024);
    }

    /* If we haven't computed enough terms yet, grow the limit. */
    while ((n - 1) >= cand_len) {
        idx_t new_limit = dp_limit * 2;
        if (new_limit < 1024) new_limit = 1024;
        if (new_limit < n * 4) new_limit = n * 4; /* rough upper bound */
        recompute(new_limit);
    }

    return (seq_t)cand_idx[n - 1];
}
