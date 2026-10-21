/*
 * A248432  Number of length n+2 0..7 arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length n+2 0..7 arrays with every three consecutive
 *            terms having the sum of some two elements equal to twice the
 *            third. Computed by a linear‑recurrence on pair states
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 28, u64 n <= 62, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Transition table for the pair state graph */
static unsigned char next_vals[64][8];
static unsigned char next_count[64];

/* Build transition table on first use */
static void build_table(void) {
    for (unsigned char p = 0; p < 8; ++p) {
        for (unsigned char q = 0; q < 8; ++q) {
            unsigned char u = (p << 3) | q;
            unsigned char cnt = 0;
            for (unsigned char r = 0; r < 8; ++r) {
                if ((p + q) == 2 * r ||
                    (p + r) == 2 * q ||
                    (q + r) == 2 * p) {
                    next_vals[u][cnt++] = r;
                }
            }
            next_count[u] = cnt;
        }
    }
}

/* Cached values of the sequence */
static seq_t *cache = NULL;    /* 0‑based: cache[0] = a(1) */
static idx_t cache_len = 0;    /* number of terms currently cached */

/* Current dynamic programming state */
static seq_t dp_cur[64];
static seq_t dp_next[64];
static idx_t current_L = 2;    /* length represented by dp_cur */

/* Make sure the transition table is built once */
static int table_built = 0;

/* Helper to extend the cache up to n */
static void ensure_cache(idx_t n) {
    if (!table_built) {
        build_table();
        table_built = 1;
        /* initialise dp_cur for length 2 (all pairs are admissible) */
        for (idx_t i = 0; i < 64; ++i) dp_cur[i] = (seq_t)1;
        current_L = 2;
    }
    /* If we already have the term, nothing to do */
    if (n <= cache_len) return;

    /* Extend the DP from current_L up to n+2 (array length L) */
    for (idx_t L = current_L + 1; L <= n + 2; ++L) {
        for (idx_t i = 0; i < 64; ++i) dp_next[i] = (seq_t)0;
        for (idx_t u = 0; u < 64; ++u) {
            seq_t val = dp_cur[u];
            if (val == (seq_t)0) continue;
            unsigned char cnt = next_count[u];
            unsigned char q = u & 7;
            for (unsigned char k = 0; k < cnt; ++k) {
                unsigned char r = next_vals[u][k];
                unsigned char v = (q << 3) | r;
                dp_next[v] += val;
            }
        }
        /* Sum all entries in dp_next: this is a(n) for n = L-2 */
        seq_t total = (seq_t)0;
        for (idx_t i = 0; i < 64; ++i) total += dp_next[i];

        /* Grow the cache array */
        idx_t new_len = cache_len + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache[cache_len] = total;
        cache_len = new_len;
        /* Prepare for next iteration */
        for (idx_t i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
        current_L = L;
    }
}

seq_t a(idx_t n) {
    /* a(1) corresponds to array length 3, so offset is handled in ensure_cache */
    ensure_cache(n);
    return cache[n - 1];
}
