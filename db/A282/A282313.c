/*
 * A282313  Number of nX5 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors.
 * https://oeis.org/A282313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A282313:
   Number of n×5 binary arrays with no 1 having more than four
   king-move neighbors. The function uses dynamic programming on
   the rows; it is valid for all n ≥ 1 and for any seq_t/idx_t
   sizes supported by the build. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ROW_MASKS 32            /* 5-bit rows : 0..31 */
#define STATE_COUNT (ROW_MASKS * ROW_MASKS)  /* prev,row pair */

/* Helper function to precompute validity of a triple (prev,curr,next) */
static bool precomp_valid[ROW_MASKS * ROW_MASKS * ROW_MASKS] = {0};

static bool valid_triple(int prev, int curr, int next)
{
    for (int j = 0; j < 5; ++j) {
        if ((curr >> j) & 1) {          /* a 1 at (row, col) */
            int cnt = 0;
            /* same row */
            if (j > 0 && (curr >> (j - 1)) & 1) ++cnt;
            if (j < 4 && (curr >> (j + 1)) & 1) ++cnt;
            /* previous row  */
            if (prev & (1 << j)) ++cnt;
            if (j > 0 && prev & (1 << (j - 1))) ++cnt;
            if (j < 4 && prev & (1 << (j + 1))) ++cnt;
            /* next row */
            if (next & (1 << j)) ++cnt;
            if (j > 0 && next & (1 << (j - 1))) ++cnt;
            if (j < 4 && next & (1 << (j + 1))) ++cnt;
            if (cnt > 4) return false;
        }
    }
    return true;
}

/* Main function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard */

    /* One-time precomputation of triple-validity table */
    static bool init = false;
    if (!init) {
        for (int prev = 0; prev < ROW_MASKS; ++prev)
            for (int curr = 0; curr < ROW_MASKS; ++curr)
                for (int next = 0; next < ROW_MASKS; ++next) {
                    int idx = (prev << 10) | (curr << 5) | next;
                    precomp_valid[idx] = valid_triple(prev, curr, next);
                }
        init = true;
    }

    /* DP arrays: dp[state] = number of ways up to current row
       where state = (prev_row << 5) | curr_row  */
    seq_t dp[STATE_COUNT] = {0};
    seq_t ndp[STATE_COUNT] = {0};

    /* Initial step: first row, prev row is zero row (0) */
    for (int curr = 0; curr < ROW_MASKS; ++curr) {
        dp[curr] = (seq_t)1;  /* state with prev=0 (shifted by 5) gives idx=curr */
    }

    /* Iterate rows 2..n */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero ndp */
        for (int i = 0; i < STATE_COUNT; ++i) ndp[i] = (seq_t)0;

        for (int prev = 0; prev < ROW_MASKS; ++prev) {
            for (int curr = 0; curr < ROW_MASKS; ++curr) {
                seq_t val = dp[(prev << 5) | curr];
                if (val == (seq_t)0) continue;
                for (int next = 0; next < ROW_MASKS; ++next) {
                    int idx = (prev << 10) | (curr << 5) | next;
                    if (precomp_valid[idx]) {
                        ndp[(curr << 5) | next] += val;
                    }
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = ndp[i];
    }

    /* Final check: last row with sentinel zero row */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < ROW_MASKS; ++prev) {
        for (int curr = 0; curr < ROW_MASKS; ++curr) {
            int idx = (prev << 10) | (curr << 5) | 0;  /* next=0 sentinel */
            if (precomp_valid[idx]) {
                result += dp[(prev << 5) | curr];
            }
        }
    }
    return result;
}
