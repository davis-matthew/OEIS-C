/*
 * A252444  Number of (n+2)X(4+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum less than 3.
 * https://oeis.org/A252444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×6 arrays with entries 0,1,2 such that
 *            every 3×3 subblock row, column, diagonal and antidiagonal sum is
 *            < 3. Computed by dynamic programming over admissible rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   reset_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reset_seq_array.h"

/* ------------------------------------------------------------------ */
/* constants                                                   */
/* ------------------------------------------------------------------ */
#define MAX_ROWS   76      /* number of admissible rows of length 6   */
#define MAX_TRANS  80      /* maximum transitions from any state     */
#define STATES      (MAX_ROWS * MAX_ROWS)

/* ------------------------------------------------------------------ */
/* tables for row digits and transitions                           */
/* ------------------------------------------------------------------ */
static uint8_t  row_digits[ MAX_ROWS ][ 6 ];                /* row i, column j */
static unsigned char trans_cnt  [ STATES ];                /* #next rows for state (a,b) */
static unsigned char trans_list [ STATES ][ MAX_TRANS ];    /* next rows list */

/* ------------------------------------------------------------------ */
/* DP arrays and cache                                              */
/* ------------------------------------------------------------------ */
static seq_t *dp        = NULL;   /* dp[a*MAX_ROWS + b] */
static seq_t *newdp     = NULL;
static seq_t *cache     = NULL;   /* cache[n] = a(n) */
static idx_t  cache_cap = 0;
static idx_t  computed_max = 0;

/* ------------------------------------------------------------------ */
/* allocation helpers (C/C++ compatible)                            */
/* ------------------------------------------------------------------ */
static void allocate_seq_array(seq_t **p, size_t n) {
#ifdef __cplusplus
    *p = new seq_t[n];
#else
    *p = (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/* build all admissible rows (length 6, digits 0..2, triple sum <=2)*/
/* ------------------------------------------------------------------ */
static void build_rows(void) {
    int id = 0;
    for (int v = 0; v < 729; ++v) {  /* 3^6 possibilities */
        int tmp = v;
        int ok  = 1;
        /* store digits most-significant first */
        for (int j = 5; j >= 0; --j) {
            row_digits[id][j] = tmp % 3;
            tmp /= 3;
        }
        /* check every 3-consecutive sum within this row */
        for (int j = 0; j <= 3; ++j) {
            int s = row_digits[id][j] + row_digits[id][j+1] + row_digits[id][j+2];
            if (s > 2) { ok = 0; break; }
        }
        if (ok) ++id;
    }
    /* id should equal MAX_ROWS */
}

/* ------------------------------------------------------------------ */
/* pre‑compute transitions between admissible rows                    */
/* ------------------------------------------------------------------ */
static void build_transitions(void) {
    for (int a = 0; a < MAX_ROWS; ++a) {
        for (int b = 0; b < MAX_ROWS; ++b) {
            int p = a * MAX_ROWS + b;
            unsigned char cnt = 0;
            for (int c = 0; c < MAX_ROWS; ++c) {
                int ok = 1;
                /* check all 3×3 blocks formed by a,b,c */
                for (int j = 0; j <= 3; ++j) {
                    /* row sums for the block */
                    int ra = row_digits[a][j] + row_digits[a][j+1] + row_digits[a][j+2];
                    int rb = row_digits[b][j] + row_digits[b][j+1] + row_digits[b][j+2];
                    int rc = row_digits[c][j] + row_digits[c][j+1] + row_digits[c][j+2];
                    if (ra > 2 || rb > 2 || rc > 2) { ok = 0; break; }
                    /* column sums */
                    for (int k = 0; k < 3; ++k) {
                        int cs = row_digits[a][j+k] + row_digits[b][j+k] + row_digits[c][j+k];
                        if (cs > 2) { ok = 0; break; }
                    }
                    if (!ok) break;
                    /* diagonals */
                    int d1 = row_digits[a][j]     + row_digits[b][j+1] + row_digits[c][j+2];
                    int d2 = row_digits[a][j+2]   + row_digits[b][j+1] + row_digits[c][j];
                    if (d1 > 2 || d2 > 2) { ok = 0; break; }
                }
                if (ok) trans_list[p][cnt++] = c;
            }
            trans_cnt[p] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* one‑time initialization                                           */
/* ------------------------------------------------------------------ */
static void init(void) {
    if (dp != NULL) return;   /* already initialized */

    build_rows();
    build_transitions();

    allocate_seq_array(&dp,    STATES);
    allocate_seq_array(&newdp, STATES);

    /* initial state: two rows chosen freely */
    for (size_t i = 0; i < STATES; ++i) dp[i] = (seq_t)1;

    /* initial cache (ready for n=0 but unused) */
    cache_cap = 10001;
    allocate_seq_array(&cache, cache_cap);
    computed_max = 0;
}

/* ------------------------------------------------------------------ */
/* extend the cache up to index 'target' (inclusive)                 */
/* ------------------------------------------------------------------ */
static void extend_cache_to(idx_t target) {
    init();
    if (target <= computed_max) return;

    idx_t current_rows = computed_max + 2;    /* current dp height */
    idx_t needed_rows  = target + 2;          /* want up to this height */

    for (; current_rows < needed_rows; ++current_rows) {
        /* newdp = 0 */
        reset_seq_array(newdp, STATES);

        /* transition from dp to newdp */
        for (int a = 0; a < MAX_ROWS; ++a) {
            for (int b = 0; b < MAX_ROWS; ++b) {
                int p = a * MAX_ROWS + b;
                seq_t val = dp[p];
                if (val == (seq_t)0) continue;
                unsigned char cnt = trans_cnt[p];
                for (unsigned char t = 0; t < cnt; ++t) {
                    int c  = trans_list[p][t];
                    int np = b * MAX_ROWS + c;
                    newdp[np] = newdp[np] + val;
                }
            }
        }

        /* swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;

        /* compute a(current_rows-1) = total of dp */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < STATES; ++i) sum = sum + dp[i];

        idx_t n = current_rows - 1;   /* number of added rows so far */
        if (n >= cache_cap) {         /* enlarge cache if needed */
            size_t new_cap = (n + 1) * 2;
            seq_t *new_cache = NULL;
#ifdef __cplusplus
            new_cache = new seq_t[new_cap];
#else
            new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
            if (!new_cache) exit(1);
            for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
            for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
            free_seq_array(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[n] = sum;
    }
    computed_max = target;
}

/* ------------------------------------------------------------------ */
/* public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    extend_cache_to(n);
    return cache[n];
}
