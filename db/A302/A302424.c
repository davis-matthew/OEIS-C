/*
 * A302424  Number of nX5 0..1 arrays with every element equal to 0, 1, 3 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with upper-left element 0 *
 *            such that for every cell the number of equal-valued horizontally
 *            and * diagonally (including anti‑diagonal) adjacent elements is
 *            0, 1, 3, or 5. * * This implementation uses dynamic programming
 *            over rows. For each * pair of consecutive...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static inline bool row_ok(int row, int prev, int next, bool has_prev, bool has_next)
{
    /* allowed equal–neighbour counts : 0, 1, 3 or 5 */
    static const bool allowed[9] = {true, true, false, true, false, true,
                                    false, false, false};

    for (int c = 0; c < 5; ++c) {
        int val   = (row >> c) & 1;
        int count = 0;

        /* horizontal neighbours */
        if (c > 0   && ((row >> (c - 1)) & 1) == val) ++count;
        if (c < 4   && ((row >> (c + 1)) & 1) == val) ++count;

        /* diagonal neighbours */
        if (has_prev) {
            if (c > 0   && ((prev >> (c - 1)) & 1) == val) ++count;
            if (c < 4   && ((prev >> (c + 1)) & 1) == val) ++count;
        }
        if (has_next) {
            if (c > 0   && ((next >> (c - 1)) & 1) == val) ++count;
            if (c < 4   && ((next >> (c + 1)) & 1) == val) ++count;
        }

        if (!allowed[count])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    const int ROWS = 32;                       /* 5‑bit rows → 0…31 */
    const size_t SIZE = (size_t)ROWS * ROWS;   /* 1024 state entries */

    seq_t *cur = alloc_seq_array(SIZE);
    seq_t *nxt = alloc_seq_array(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
        cur[i] = nxt[i] = (seq_t)0;
    }

    /* Special case n == 1 */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int r0 = 0; r0 < ROWS; ++r0) {
            if (r0 & 1)        /* upper‑left must be 0 (bit 0) */
                continue;
            if (row_ok(r0, -1, -1, false, false))
                ans += (seq_t)1;
        }
        free_seq_array(cur);
        free_seq_array(nxt);
        return ans;
    }

    /* Initialise states for first two rows */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        if (r0 & 1)          /* upper‑left must be 0 */
            continue;
        for (int r1 = 0; r1 < ROWS; ++r1) {
            if (!row_ok(r0, -1, r1, false, true))
                continue;
            cur[(size_t)r0 * ROWS + r1] = (seq_t)1;
        }
    }

    /* DP for rows 3 … n */
    for (idx_t row = 2; row < n; ++row) {
        for (size_t i = 0; i < SIZE; ++i)
            nxt[i] = (seq_t)0;

        for (int prev = 0; prev < ROWS; ++prev) {
            for (int currow = 0; currow < ROWS; ++currow) {
                seq_t ways = cur[(size_t)prev * ROWS + currow];
                if (!ways)
                    continue;
                for (int nextrow = 0; nextrow < ROWS; ++nextrow) {
                    if (!row_ok(currow, prev, nextrow, true, true))
                        continue;
                    size_t idx = (size_t)currow * ROWS + nextrow;
                    nxt[idx] += ways;
                }
            }
        }
        /* swap cur and nxt */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum over final states – validate the last row */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int last = 0; last < ROWS; ++last) {
            seq_t ways = cur[(size_t)prev * ROWS + last];
            if (!ways)
                continue;
            if (!row_ok(last, prev, -1, true, false))
                continue;
            result += ways;
        }
    }

    free_seq_array(cur);
    free_seq_array(nxt);
    return result;
}
