/*
 * A295805  Numbers that have exactly eight representations as a sum of seven
 *          positive squares.
 * https://oeis.org/A295805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth integer that has exactly eight unordered
 *            representations as a sum of seven positive squares. The function
 *            counts representations with a DP over square values, expanding
 *            the tables on demand
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* DP tables for k=0..7. dp[k][v] = number of ways to represent v
   as a sum of k positive squares (unordered). */
static uint32_t *dp[8] = {NULL};

/* Maximum value for which DP is computed. */
static idx_t current_limit = 0;
/* Largest square index processed. */
static idx_t current_sqrt = 0;

/* Bitmap noting which values have already been recorded as terms (dp[7]==8). */
static unsigned char *term_seen = NULL;
static idx_t term_seen_limit = 0;

/* Array of found terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Integer square root: floor_sqrt_u64(n) returns largest s with s*s <= n. */
static inline idx_t floor_sqrt_u64(idx_t n) {
    idx_t low = 0, high = 1;
    while ((uint64_t)high * high <= n) high <<= 1;
    low = high >> 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((uint64_t)mid * mid <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Initialises the DP tables and helper arrays. */
static void init_sieve(void) {
    static int init_done = 0;
    if (init_done) return;
    for (int k = 0; k < 8; ++k) {
        dp[k] = (uint32_t *)calloc(1, sizeof(uint32_t));
        if (!dp[k]) exit(1);
    }
    dp[0][0] = 1;
    term_seen = (unsigned char *)calloc(1, sizeof(unsigned char));
    if (!term_seen) exit(1);
    term_seen_limit = 0;
    init_done = 1;
}

/* Expand the sieve up to at least new_limit. */
static void expand_sieve(idx_t new_limit) {
    if (new_limit <= current_limit) return;
    init_sieve();

    /* Reallocate DP arrays to the new size and zero new entries. */
    idx_t old_limit = current_limit;
    for (int k = 0; k < 8; ++k) {
        uint32_t *ptr = (uint32_t *)realloc(dp[k], (new_limit + 1) * sizeof(uint32_t));
        if (!ptr) exit(1);
        dp[k] = ptr;
        if (new_limit > old_limit) {
            memset(dp[k] + old_limit + 1, 0, (new_limit - old_limit) * sizeof(uint32_t));
        }
    }

    /* Extend term_seen bitmap. */
    if (new_limit > term_seen_limit) {
        unsigned char *ptr = (unsigned char *)realloc(term_seen, (new_limit + 1) * sizeof(unsigned char));
        if (!ptr) exit(1);
        term_seen = ptr;
        memset(term_seen + term_seen_limit + 1, 0, (new_limit - term_seen_limit));
        term_seen_limit = new_limit;
    }

    /* Process new squares from current_sqrt+1 up to floor_sqrt(new_limit). */
    idx_t new_sqrt = floor_sqrt_u64(new_limit);
    for (idx_t s = current_sqrt + 1; s <= new_sqrt; ++s) {
        idx_t sq = s * s;
        if (sq > new_limit) break;
        for (int k = 1; k <= 7; ++k) {
            for (idx_t v = sq; v <= new_limit; ++v) {
                dp[k][v] += dp[k - 1][v - sq];
            }
        }
    }

    /* Record any new terms that now have exactly 8 representations. */
    for (idx_t v = 1; v <= new_limit; ++v) {
        if (!term_seen[v] && dp[7][v] == 8) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 1;
                seq_t *newt = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!newt) exit(1);
                terms = newt;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)v;
            term_seen[v] = 1;
        }
    }

    current_limit = new_limit;
    current_sqrt = new_sqrt;
}

/* Ensures that at least n terms have been computed. */
static void ensure_n_terms(idx_t n) {
    if (n == 0) return;
    if (terms_len >= n) return;
    idx_t limit = current_limit ? current_limit : 1;
    while (terms_len < n) {
        idx_t new_limit = limit * 2 + 100;
        if (new_limit <= limit) new_limit = limit + 100;   /* safety against overflow */
        expand_sieve(new_limit);
        limit = new_limit;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_n_terms(n);
    if (n <= terms_len) return terms[n - 1];
    return 0;  /* out of range; should not happen for valid n */
}
