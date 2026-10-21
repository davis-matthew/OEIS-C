/*
 * A283128  Number of nX6 0..1 arrays with no 1 equal to more than two of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A283128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A283128
 *  Number of n×6 0..1 arrays with no 1 having more than two horizontal
 *  or vertical neighbours.
 *
 *  The function computes the sequence by dynamic programming using a
 *  state transition graph for the 6‑column width.  Each state consists
 *  of the mask of 1s in the previous row and a set of columns that
 *  are forbidden for the next row (columns where a 1 would have
 *  exactly two neighbours).  This representation has 3^6 = 729
 *  reachable states; all transitions are pre‑computed once.
 *
 *  The code is fully compatible with C11 and C++17, and works for any
 *  `seq_t` type (plain integer or an arbitrary‑precision class).  It
 *  allocates dynamic memory for the DP arrays only once and guarantees
 *  proper construction when used from C++.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* width of the arrays */
#define NUM_ROWS 6
/* 6 bits: up to 64 masks, 6 bits for forbid bits → 12‑bit key */
#define MAX_STATE_KEY ((1 << (NUM_ROWS * 2)))
#define NUM_STATES    729          /* 3^6 reachable states */

/* maximum outgoing transitions from a state (empirically ≤64) */
#define MAX_TRANS     64

/* pre‑computed data */
static bool initialized = false;
/* key → state index */
static int key_to_index[MAX_STATE_KEY] = {0};
/* state properties */
static unsigned mask_of[NUM_STATES];
static unsigned forbid_of[NUM_STATES];
/* transition lists */
static int trans_cnt[NUM_STATES];
static int trans_dst[NUM_STATES][MAX_TRANS];

/* Allocate and default‑construct DP arrays once */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* helper to build all states and transitions */
static void precompute(void)
{
    /* enumerate all states (mask, forbid subset of mask) */
    int idx = 0;
    for (unsigned mask = 0; mask < (1u << NUM_ROWS); ++mask) {
        unsigned sub = mask;
        do {
            int key = (mask << NUM_ROWS) | sub;
            key_to_index[key] = idx;
            mask_of[idx]     = mask;
            forbid_of[idx]   = sub;
            ++idx;
            if (sub == 0) break;
            sub = (sub - 1U) & mask;
        } while (true);
    }

    /* build transition lists */
    for (int src = 0; src < NUM_STATES; ++src) {
        unsigned m1  = mask_of[src];
        unsigned f1  = forbid_of[src];
        int count = 0;
        for (unsigned m2 = 0; m2 < (1u << NUM_ROWS); ++m2) {
            /* forbid bits from previous row must not be used */
            if ((m2 & f1) != 0) continue;

            /* check that each 1 in m2 does not have >2 horizontal/vertical neighbours */
            bool ok = true;
            for (int j = 0; j < NUM_ROWS && ok; ++j) {
                if ((m2 >> j) & 1U) {
                    int left  = (j > 0)   && ((m2 >> (j - 1)) & 1U);
                    int right = (j < NUM_ROWS - 1) && ((m2 >> (j + 1)) & 1U);
                    int up    = (m1 >> j) & 1U;
                    if (left + right + up > 2) ok = false;
                }
            }
            if (!ok) continue;

            /* forbid columns for the next row: columns where a 1 has exactly two neighbours */
            unsigned f2 = 0;
            for (int j = 0; j < NUM_ROWS; ++j) {
                if ((m2 >> j) & 1U) {
                    int left  = (j > 0)   && ((m2 >> (j - 1)) & 1U);
                    int right = (j < NUM_ROWS - 1) && ((m2 >> (j + 1)) & 1U);
                    int up    = (m1 >> j) & 1U;
                    if (left + right + up == 2) f2 |= (1u << j);
                }
            }
            int key2 = (m2 << NUM_ROWS) | f2;
            int tgt = key_to_index[key2];
            trans_dst[src][count++] = tgt;
        }
        trans_cnt[src] = count;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!initialized) {
        precompute();
        dp_cur = (seq_t*)malloc(NUM_STATES * sizeof(seq_t));
        dp_next = (seq_t*)malloc(NUM_STATES * sizeof(seq_t));
        if (!dp_cur || !dp_next) return 0;    /* memory failure */

#ifdef __cplusplus
        /* construct objects for user-defined seq_t */
        for (int i = 0; i < NUM_STATES; ++i) new (&dp_cur[i]) seq_t();
        for (int i = 0; i < NUM_STATES; ++i) new (&dp_next[i]) seq_t();
#endif
        initialized = true;
    }

    /* clear current DP */
    for (int i = 0; i < NUM_STATES; ++i) dp_cur[i] = 0;

    /* initialise first row */
    for (unsigned mask = 0; mask < (1u << NUM_ROWS); ++mask) {
        unsigned forbid = 0;
        for (int j = 0; j < NUM_ROWS; ++j) {
            if ((mask >> j) & 1U) {
                int left  = (j > 0)   && ((mask >> (j - 1)) & 1U);
                int right = (j < NUM_ROWS - 1) && ((mask >> (j + 1)) & 1U);
                if (left + right == 2) forbid |= (1u << j);
            }
        }
        int key  = (mask << NUM_ROWS) | forbid;
        int idx  = key_to_index[key];
        dp_cur[idx] += 1;    /* each first-row configuration counts as one */
    }

    /* DP over remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < NUM_STATES; ++i) dp_next[i] = 0;

        for (int src = 0; src < NUM_STATES; ++src) {
            seq_t val = dp_cur[src];
            if (val == 0) continue;
            int count = trans_cnt[src];
            for (int t = 0; t < count; ++t) {
                int tgt = trans_dst[src][t];
                dp_next[tgt] += val;
            }
        }
        /* swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states to obtain a(n) */
    seq_t total = 0;
    for (int i = 0; i < NUM_STATES; ++i) total += dp_cur[i];
    return total;
}
