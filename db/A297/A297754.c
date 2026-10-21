/*
 * A297754  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 3 or 4 neighboring 1s.
 * https://oeis.org/A297754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   triple_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  A297754: Number of 6×n binary arrays in which every 1 has exactly
 *  1, 3 or 4 adjacent 1s counting only horizontal and diagonal
 *  (anti‑diagonal) neighbours (no vertical neighbours).
 *
 *  The function obtains the value via a column‑by‑column transfer‑matrix
 *  dynamic programming on the 6‑bit column state space.
 *  Complexity:  O(n·64²) elementary operations, well within the limits
 *  for n ≤ 10000.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/triple_valid.h"

/* ------------------------------------------------------------------------- */
/*  Constants and internal data structures                                  */
/* ------------------------------------------------------------------------- */
#define STATES   64      /* 6‑bit column patterns: 0 … 63                 */
#define MAX_TRANS 64     /* at most 64 possible right columns per pair   */

typedef struct {
    uint64_t mask;              /* bitmask of admissible right columns        */
    int cnt;                    /* number of transitions stored in list[]      */
    unsigned char list[MAX_TRANS]; /* indices of admissible right columns   */
} Trans;

static Trans trans[STATES * STATES];   /* 4096 entries                        */
static bool trans_init = false;        /* transitions computed flag            */

/* ------------------------------------------------------------------------- */
/*  Check validity of a triple (left, mid, right)                           */

/* ------------------------------------------------------------------------- */
/*  Build all transitions for the DP matrix                                 */
/* ------------------------------------------------------------------------- */
static void init_transitions(void)
{
    for (int left = 0; left < STATES; ++left) {
        for (int mid = 0; mid < STATES; ++mid) {
            int idx = (left << 6) | mid;
            Trans *t = &trans[idx];
            t->mask = 0;
            t->cnt  = 0;
            for (int right = 0; right < STATES; ++right) {
                if (triple_valid(left, mid, right)) {
                    t->mask |= (uint64_t)1ULL << right;
                    t->list[t->cnt++] = (unsigned char)right;
                }
            }
        }
    }
    trans_init = true;
}

/* ------------------------------------------------------------------------- */
/*  Main function computing the sequence                                    */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* behaviour below offset is irrelevant   */

    if (!trans_init) init_transitions(); /* ensure transition table is ready       */

    /* Two DP vectors of size STATES*STATES (4096) */
    static seq_t dp0[STATES * STATES];
    static seq_t dp1[STATES * STATES];
    seq_t *dp_cur  = dp0;
    seq_t *dp_next = dp1;

    /* initialise: first column with left side (virtual) = 0            */
    for (int i = 0; i < STATES * STATES; ++i) dp_cur[i] = 0;
    for (int b = 0; b < STATES; ++b) dp_cur[b] = 1;   /* state (0 , b) */

    /* Extend for the remaining columns */
    idx_t steps = n - 1;
    for (idx_t step = 0; step < steps; ++step) {
        /* clear next DP */
        for (int i = 0; i < STATES * STATES; ++i) dp_next[i] = 0;

        /* transition */
        for (int idx = 0; idx < STATES * STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            Trans *t = &trans[idx];
            int mid = idx & 63; /* the column that becomes the new left */
            for (int k = 0; k < t->cnt; ++k) {
                unsigned char nxt = t->list[k];
                int next_idx = (mid << 6) | nxt; /* (previous mid, new right) */
                dp_next[next_idx] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Final dummy column (right side = 0) to validate the last real column */
    for (int i = 0; i < STATES * STATES; ++i) dp_next[i] = 0;
    for (int idx = 0; idx < STATES * STATES; ++idx) {
        seq_t val = dp_cur[idx];
        if (val == 0) continue;
        Trans *t = &trans[idx];
        if (t->mask & 1ULL) {            /* transition to right=0 possible?   */
            int mid = idx & 63;          /* last real column                 */
            int next_idx = (mid << 6);   /* (mid, 0)                          */
            dp_next[next_idx] += val;
        }
    }
    /* swap to make dp_cur hold the final states */
    seq_t *tmp = dp_cur;
    dp_cur = dp_next;
    dp_next = tmp;

    /* Sum all states whose last column is 0 */
    seq_t result = 0;
    for (int prev = 0; prev < STATES; ++prev) {
        result += dp_cur[prev << 6];
    }
    return result;
}
