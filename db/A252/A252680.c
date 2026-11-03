/*
 * A252680  Number of (1+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 2 5 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 2 5 6 or 7.
 * https://oeis.org/A252680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 3×(n+2) arrays with entries 0‑3 such that
 *            * every 3×3 subblock has all row sums and the main diagonal sum
 *            belonging * to {0,2,5,6,7}, while none of its column sums or the
 *            anti‑diagonal sum * belongs to that same set. * The function
 *            below computes a(n) by dynamic programming...
 * Range:     u8 none, u16 n <= 12, u32 n <= 33, u64 n <= 74, u128 n <= 155,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 198 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Decode a column index (0..63) into its three entries (rows 0,1,2).   */
static inline void decode_col(int col, unsigned char rows[3]) {
    rows[0] = col & 3;
    rows[1] = (col >> 2) & 3;
    rows[2] = (col >> 4) & 3;
}

/* Test whether a 3×3 block given by columns c0,c1,c2 satisfies the
 * constraints.  Columns are numbered from left to right.                  */
static bool block_ok(int c0, int c1, int c2) {
    unsigned char r0[3], r1[3], r2[3];
    decode_col(c0, r0);
    decode_col(c1, r1);
    decode_col(c2, r2);

    /* Allowed sums: 0,2,5,6,7 */
    const uint16_t allowed_mask = (1u << 0) | (1u << 2) |
                                 (1u << 5) | (1u << 6) | (1u << 7);

    /* Row sums must be allowed */
    for (int i = 0; i < 3; ++i) {
        int sum = r0[i] + r1[i] + r2[i];
        if (!(allowed_mask & (1u << sum))) return false;
    }

    /* Main diagonal must be allowed */
    int diag_main = r0[0] + r1[1] + r2[2];
    if (!(allowed_mask & (1u << diag_main))) return false;

    /* Anti‑diagonal must NOT be allowed */
    int diag_anti = r0[2] + r1[1] + r2[0];
    if (allowed_mask & (1u << diag_anti)) return false;

    /* Column sums must NOT be allowed ------------------------------- */
    /* Correct column sums: sum over rows of each individual column     */
    int col_sums[3];
    col_sums[0] = r0[0] + r0[1] + r0[2];
    col_sums[1] = r1[0] + r1[1] + r1[2];
    col_sums[2] = r2[0] + r2[1] + r2[2];
    for (int j = 0; j < 3; ++j) {
        int sum = col_sums[j];
        if (allowed_mask & (1u << sum)) return false;
    }

    return true;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Transition tables: 4096 states (c0,c1). Each entry lists all c2 such that
 * (c0,c1,c2) is a valid 3×3 block.                                      */
#define STATES 4096
#define MAX_TRANS 262144   /* upper bound: 4096 * 64 */

static int *trans_pool;            /* contiguous pool of next columns   */
static int trans_start[STATES];    /* start index in pool for state    */
static int trans_len[STATES];      /* number of valid transitions      */

/* DP buffers.  They are of type seq_t so they work for arbitrary-precision
 * builds as well as native integer builds.                               */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Memoised sequence values: cache[1] = a(1).  1‑based indexing.         */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* allocated capacity (excluding index 0) */
static idx_t max_cached = 0;   /* highest n for which cache[n] is set */

/* Number of columns processed so far.  Initial width is 3 (n = 1).          */
static idx_t current_width = 3;

/* --------------------------------------------------------------------- */
/* Initialise transition tables and DP buffers.                             */
static void init_tables(void) {
    trans_pool = (int *)malloc(MAX_TRANS * sizeof(int));
    if (!trans_pool) exit(EXIT_FAILURE);

    int pool_offset = 0;
    for (int s = 0; s < STATES; ++s) {
        int c0 = s / 64;
        int c1 = s % 64;
        trans_start[s] = pool_offset;
        trans_len[s]   = 0;
        for (int c2 = 0; c2 < 64; ++c2) {
            if (block_ok(c0, c1, c2)) {
                trans_pool[pool_offset++] = c2;
                ++trans_len[s];
            }
        }
    }

    dp_cur  = mem_new(seq_t, STATES);
    dp_next = mem_new(seq_t, STATES);
    if (!dp_cur || !dp_next) exit(EXIT_FAILURE);

    /* For width = 3 (one block), each valid triple contributes 1 to the
     * state corresponding to its last two columns.                            */
    for (int s = 0; s < STATES; ++s) dp_cur[s] = (seq_t)0;
    for (int c0 = 0; c0 < 64; ++c0) {
        for (int c1 = 0; c1 < 64; ++c1) {
            for (int c2 = 0; c2 < 64; ++c2) {
                if (block_ok(c0, c1, c2)) {
                    int state = c1 * 64 + c2;
                    dp_cur[state] += (seq_t)1;
                }
            }
        }
    }

    /* Initial cache setup for n = 1 (width = 3) */
    cache = mem_new(seq_t, 2); /* index 0 unused, index 1 for a(1) */
    cache_size = 1;
    max_cached = 1;
    /* compute total for width 3 */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i) total += dp_cur[i];
    cache[1] = total;
}

/* --------------------------------------------------------------------- */
/* Ensure that values up to n (1‑based) have been computed.                */
static void ensure_computed(idx_t n) {
    if (n <= max_cached) return;

    if (!dp_cur) init_tables();   /* lazy initialisation */

    while (max_cached < n) {
        /* Add one column → new width = current_width + 1 */
        ++current_width;

        /* zero dp_next (safe for integer or arbitrary‑precision types) */
        for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            int start = trans_start[s];
            int len   = trans_len[s];
            int next_state_offset = s % 64;          /* c1 */
            for (int k = 0; k < len; ++k) {
                int c2 = trans_pool[start + k];
                int next_state = next_state_offset * 64 + c2;   /* (c1,c2) */
                dp_next[next_state] += val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* compute total number of arrays for this width */
        seq_t total = (seq_t)0;
        for (int i = 0; i < STATES; ++i) total += dp_cur[i];

        /* store into cache (sequence index = width - 2 = current_width - 2) */
        ++max_cached;   /* now corresponds to n = max_cached */
        if (max_cached > cache_size) {
            idx_t new_cap = (cache_size == 0) ? 64 : cache_size * 2;
            while (new_cap < max_cached) new_cap <<= 1;
            seq_t *new_cache = mem_resize(seq_t, cache, (new_cap + 1));
            if (!new_cache) exit(EXIT_FAILURE);
            cache = new_cache;
            cache_size = new_cap;
        }
        cache[max_cached] = total;
    }
}

/* --------------------------------------------------------------------- */
/* Public function required by the build system.                           */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_computed(n);
    return cache[n];
}
