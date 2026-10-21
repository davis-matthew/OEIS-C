/*
 * A250430  Number of (n+1)X(6+1) 0..1 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)x7 binary arrays with nondecreasing sum of
 *            every two consecutive values in every row and column
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 91, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* Internal tables and state -------------------------------------------- */

static bool seq_initialized = false;
static size_t seq_allowed_cnt    = 0;                /* # of allowed rows   */
static uint8_t *seq_allowed_rows = NULL;            /* allowed row masks   */
static int *seq_row_to_idx       = NULL;            /* mapping 0..127 -> index or -1 */

static size_t seq_state_cnt     = 0;                /* allowed_cnt^2       */
static int *seq_deg             = NULL;            /* number of successors per state */
static int *seq_adj             = NULL;            /* adjacency table [state][nextIdx] */

static seq_t *seq_cur = NULL;                      /* current DP state    */
static seq_t *seq_next = NULL;                     /* next DP state       */

static seq_t *seq_cache = NULL;                    /* cache of a(n) values */
static size_t seq_cache_len = 0;                   /* size of cache       */
static size_t seq_last_n = 0;                      /* largest n already computed */

/* --------------------------------------------------------------------- */

static void seq_init(void) {
    const int FULLR = 128;                           /* 7-bit rows          */
    int row_to_idx[128];
    for (int i = 0; i < FULLR; ++i) row_to_idx[i] = -1;
    int allowed[128];
    int allowed_cnt = 0;

    /* ------------------------------------------------------------------ */
    /*  Find all 7‑bit rows whose consecutive‑pair sums are nondecreasing */
    /* ------------------------------------------------------------------ */
    for (int r = 0; r < FULLR; ++r) {
        int sums[6];
        for (int j = 0; j < 6; ++j) {
            int b1 = (r >> j) & 1;
            int b2 = (r >> (j + 1)) & 1;
            sums[j] = b1 + b2;
        }
        bool ok = true;
        for (int j = 0; j < 5; ++j)
            if (sums[j] > sums[j + 1]) { ok = false; break; }
        if (ok) {
            allowed[allowed_cnt++] = r;
            row_to_idx[r] = allowed_cnt - 1;
        }
    }
    seq_allowed_cnt = allowed_cnt;
    seq_allowed_rows = (uint8_t*)malloc(seq_allowed_cnt * sizeof(uint8_t));
    for (size_t i = 0; i < seq_allowed_cnt; ++i)
        seq_allowed_rows[i] = (uint8_t)allowed[i];
    seq_row_to_idx = (int*)malloc(FULLR * sizeof(int));
    memcpy(seq_row_to_idx, row_to_idx, FULLR * sizeof(int));

    /* ------------------------------------------------------------------ */
    /*  Build transition graph                                            */
    /* ------------------------------------------------------------------ */
    seq_state_cnt = seq_allowed_cnt * seq_allowed_cnt;
    seq_deg = (int*)calloc(seq_state_cnt, sizeof(int));
    seq_adj = (int*)malloc(seq_state_cnt * seq_allowed_cnt * sizeof(int)); /* flat */

    for (int i = 0; i < seq_allowed_cnt; ++i) {
        int prev = allowed[i];
        for (int j = 0; j < seq_allowed_cnt; ++j) {
            int curr = allowed[j];
            size_t stateIdx = (size_t)i * seq_allowed_cnt + j;
            size_t offset = stateIdx * seq_allowed_cnt;

            /* Sums of the current pair (prev, curr) */
            int sumPrev[7];
            for (int c = 0; c < 7; ++c)
                sumPrev[c] = ((prev >> c) & 1) + ((curr >> c) & 1);

            /* Try every possible next row */
            for (int k = 0; k < seq_allowed_cnt; ++k) {
                int next = allowed[k];
                bool good = true;
                for (int c = 0; c < 7; ++c) {
                    int sumNew = ((curr >> c) & 1) + ((next >> c) & 1);
                    if (sumNew < sumPrev[c]) { good = false; break; }
                }
                if (good) {
                    seq_adj[offset + seq_deg[stateIdx]] = k;
                    seq_deg[stateIdx]++;
                }
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /*  DP setup for n = 1 (two rows)                                     */
    /* ------------------------------------------------------------------ */
    seq_cur  = (seq_t*)malloc(seq_state_cnt * sizeof(seq_t));
    seq_next = (seq_t*)malloc(seq_state_cnt * sizeof(seq_t));
    for (size_t s = 0; s < seq_state_cnt; ++s)
        seq_cur[s] = 1;                                 /* one way to form each pair */

    seq_cache_len = 4;                                  /* first small size */
    seq_cache = (seq_t*)malloc(seq_cache_len * sizeof(seq_t));
    seq_cache[1] = 0;
    for (size_t s = 0; s < seq_state_cnt; ++s)
        seq_cache[1] += seq_cur[s];

    seq_last_n = 1;
    seq_initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Extend DP to compute a(n) for all n <= target                         */
/* --------------------------------------------------------------------- */
static void seq_recalc_upto(idx_t target) {
    if (target <= seq_last_n) return;

    for (size_t step = seq_last_n + 1; step <= (size_t)target; ++step) {
        /* zero next state buffer */
        memset(seq_next, 0, seq_state_cnt * sizeof(seq_t));

        /* transition */
        for (size_t s = 0; s < seq_state_cnt; ++s) {
            seq_t val = seq_cur[s];
            if (val == 0) continue;

            int j = (int)(s % seq_allowed_cnt);  /* current row index */
            size_t offset = s * seq_allowed_cnt;
            int deg_s = seq_deg[s];

            for (int d = 0; d < deg_s; ++d) {
                int nextIdx = seq_adj[offset + d];
                size_t newState = ((size_t)j * seq_allowed_cnt) + (size_t)nextIdx;
                seq_next[newState] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp = seq_cur; seq_cur = seq_next; seq_next = tmp;

        /* compute total for this n */
        seq_t total = 0;
        for (size_t s = 0; s < seq_state_cnt; ++s)
            total += seq_cur[s];

        /* enlarge cache if necessary */
        if ((size_t)step >= seq_cache_len) {
            size_t newSize = seq_cache_len * 2;
            seq_t *newCache = (seq_t*)realloc(seq_cache, newSize * sizeof(seq_t));
            seq_cache = newCache;
            seq_cache_len = newSize;
        }
        seq_cache[step] = total;
    }
    seq_last_n = target;
}

/* --------------------------------------------------------------------- */
/*  Public interface ---------------------------------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (!seq_initialized)
        seq_init();

    if (n < 1)
        return 0;                            /* undefined for n = 0 */

    if ((size_t)n >= seq_cache_len) {
        size_t newSize = seq_cache_len;
        while ((size_t)n >= newSize)
            newSize *= 2;
        seq_t *newCache = (seq_t*)realloc(seq_cache, newSize * sizeof(seq_t));
        seq_cache = newCache;
        seq_cache_len = newSize;
    }

    seq_recalc_upto(n);
    return seq_cache[(size_t)n];
}
