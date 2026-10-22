/*
 * A302893  Number of 6Xn 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays where each element has a
 *            count of equal neighbours in the six directions (left, right,
 *            up-left, up-right, down-left, down-right) equal to 0,2,3,4 or 5.
 *            The array has its upper-left element fixed to 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_ROWS    6
#define MASKS       (1 << NUM_ROWS)           /* 64  */
#define NONE        MASKS                      /* 64  */
#define PREV_CNT    (MASKS + 1)               /* 65  */
#define STATE_CNT   (PREV_CNT * MASKS)        /* 4160  */

/* ------------------------------------------------------------------ */
/* Validity test for one column given its left and right neighbours   */
/* ------------------------------------------------------------------ */
static inline bool
col_valid(int col, int left, int right, bool left_present, bool right_present)
{
    for (int r = 0; r < NUM_ROWS; ++r) {
        int v = (col >> r) & 1;
        int count = 0;
        if (left_present) {
            count += ((left >> r) & 1) == v;
            if (r > 0)   count += ((left >> (r - 1)) & 1) == v;
            if (r < NUM_ROWS - 1) count += ((left >> (r + 1)) & 1) == v;
        }
        if (right_present) {
            count += ((right >> r) & 1) == v;
            if (r > 0)   count += ((right >> (r - 1)) & 1) == v;
            if (r < NUM_ROWS - 1) count += ((right >> (r + 1)) & 1) == v;
        }
        if (count == 1)          return false;
        if (right_present && count == 6)  return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Transition tables, built once                                     */
/* ------------------------------------------------------------------ */
static bool  tables_built = false;
static uint8_t  next_list[STATE_CNT][MASKS];
static uint8_t  next_len[STATE_CNT];
static uint8_t  valid_last[PREV_CNT][MASKS];

static void
build_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    for (int prev = 0; prev < PREV_CNT; ++prev) {
        bool left_present = (prev != NONE);
        int left_mask = (prev == NONE ? 0 : prev);

        for (int curr = 0; curr < MASKS; ++curr) {
            int s = prev * MASKS + curr;
            uint8_t cnt = 0;

            /* Transitions to next column (right present) */
            for (int next = 0; next < MASKS; ++next) {
                if (col_valid(curr, left_mask, next, left_present, true))
                    next_list[s][cnt++] = (uint8_t)next;
            }
            next_len[s] = cnt;

            /* Validation when this column is the last one (right absent) */
            valid_last[prev][curr] = col_valid(curr, left_mask, 0, left_present, false);
        }
    }
}

/* ------------------------------------------------------------------ */
/* DP and caching                                                      */
/* ------------------------------------------------------------------ */
static seq_t *dp_cur   = NULL;
static seq_t *dp_next  = NULL;

static idx_t   max_cached = 0;
static seq_t *cache      = NULL;
static size_t  cache_cap  = 0;

static inline size_t
state_index(int prev, int cur)
{
    return (size_t)prev * MASKS + cur;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    build_tables();

    /* Allocate or resize cache as needed */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        memset(cache, 0, cache_cap * sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        memset(cache + max_cached + 1, 0,
               (cache_cap - (max_cached + 1)) * sizeof(seq_t));
    }

    /* Return already computed value */
    if (n <= max_cached) return cache[n];

    /* Initialise DP for column 1 (prev = NONE) */
    if (dp_cur == NULL) {
        dp_cur  = (seq_t *)malloc(STATE_CNT * sizeof(seq_t));
        dp_next = (seq_t *)malloc(STATE_CNT * sizeof(seq_t));
        memset(dp_cur, 0, STATE_CNT * sizeof(seq_t));

        /* First column: only masks with upper-left bit 0 are allowed */
        for (int cur = 0; cur < MASKS; ++cur) {
            if ((cur & 1) == 0)         /* upper-left (row 0) must be 0 */
                dp_cur[state_index(NONE, cur)] = (seq_t)1;
        }

        /* Compute a(1) directly (single column, no neighbours) */
        seq_t a1 = 0;
        for (int cur = 0; cur < MASKS; ++cur) {
            if ((cur & 1) == 0 && col_valid(cur, 0, 0, false, false))
                a1 += (seq_t)1;
        }
        cache[1] = a1;
        max_cached = 1;
        if (n == 1) return a1;
    }

    /* Incrementally compute further values until n is reached */
    for (idx_t col = max_cached + 1; col <= n; ++col) {
        /* Transition from state (prev,cur) to (cur,next) */
        memset(dp_next, 0, STATE_CNT * sizeof(seq_t));

        for (int prev = 0; prev < PREV_CNT; ++prev) {
            for (int cur = 0; cur < MASKS; ++cur) {
                seq_t val = dp_cur[state_index(prev, cur)];
                if (val == (seq_t)0) continue;
                size_t s = state_index(prev, cur);
                uint8_t cnt = next_len[s];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int next = next_list[s][k];
                    seq_t *target = &dp_next[state_index(cur, next)];
                    *target += val;
                }
            }
        }

        /* Sum over states where the last column is valid (right absent) */
        seq_t total = 0;
        for (int prev = 0; prev < PREV_CNT; ++prev) {
            for (int cur = 0; cur < MASKS; ++cur) {
                if (valid_last[prev][cur]) {
                    seq_t val = dp_next[state_index(prev, cur)];
                    if (val != (seq_t)0) total += val;
                }
            }
        }

        cache[col] = total;
        max_cached = (idx_t)col;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return cache[n];
}
