/*
 * A345542  Numbers that are the sum of nine positive cubes in three or more
 *          ways.
 * https://oeis.org/A345542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate sums of nine positive cubes and collect numbers
   that are representable in at least three distinct ways. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t    current_limit = 0;
static seq_t  *term_list     = NULL;
static size_t  term_len      = 0;
static size_t  term_capacity = 0;

/* Build all terms up to new_limit.  Returns the number of terms found. */
static size_t build_terms(idx_t new_limit)
{
    /* Free any previous list of terms. */
    if (term_list) { free(term_list); term_list = NULL; }
    term_len = 0; term_capacity = 0;

    /* Allocate DP arrays for k = 0..9. */
    static uint8_t *dp[10];
    for (int k = 0; k < 10; ++k) {
        dp[k] = (uint8_t*)calloc((size_t)new_limit + 1, sizeof(uint8_t));
        if (!dp[k]) abort();
    }
    dp[0][0] = 1; /* one way to make 0 with 0 cubes */

    /* Collect all cube values ≤ new_limit. */
    uint32_t cube_vals[200];
    size_t cube_cnt = 0;
    for (uint32_t i = 1; ; ++i) {
        uint64_t c = (uint64_t)i * i * i;
        if (c > new_limit) break;
        cube_vals[cube_cnt++] = (uint32_t)c;
    }

    /* DP: count unordered partitions (repetition allowed). */
    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        uint32_t c = cube_vals[ci];
        for (idx_t sum = c; sum <= new_limit; ++sum) {
            size_t si = (size_t)sum;
            size_t pi = si - c;
            for (int k = 1; k <= 9; ++k) {
                uint8_t val = dp[k][si] + dp[k-1][pi];
                if (val > 3) val = 3;            /* cap at 3 */
                dp[k][si] = val;
            }
        }
    }

    /* Reserve space for resulting terms. */
    size_t est_cap = new_limit / 65 + 10;          /* ≈1.5 % of limit */
    if (est_cap < 1024) est_cap = 1024;
    term_list = (seq_t*)realloc(term_list, est_cap * sizeof(seq_t));
    if (!term_list) abort();
    term_capacity = est_cap;
    term_len = 0;

    /* Extract the terms with at least 3 representations. */
    for (idx_t s = 1; s <= new_limit; ++s) {
        if (dp[9][(size_t)s] >= 3) {
            if (term_len == term_capacity) {
                term_capacity *= 2;
                seq_t *tmp = (seq_t*)realloc(term_list, term_capacity * sizeof(seq_t));
                if (!tmp) abort();
                term_list = tmp;
            }
            term_list[term_len++] = (seq_t)s;
        }
    }

    /* Clean up DP arrays. */
    for (int k = 0; k < 10; ++k) {
        free(dp[k]); dp[k] = NULL;
    }

    current_limit = new_limit;
    return term_len;
}

/* Ensure the cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (term_len >= n) return;

    idx_t limit = (current_limit == 0) ? 200000 : current_limit;
    while (term_len < n) {
        limit = limit * 2;          /* double the search limit */
        build_terms(limit);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑indexed */
    ensure_terms(n);
    if (n > term_len) return 0;    /* safety: n too large for built range */
    return term_list[n - 1];
}
