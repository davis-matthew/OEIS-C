/*
 * A299687  Number of nX6 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A299687 – Number of n×6 0/1 arrays with upper‑left element 0
    and no element having exactly 2 or 8 king‑move adjacent elements
    with the same value.  The implementation uses a DP over pairs of
    consecutive rows.  The upper‑left cell of the first row is forced to
    be 0.  The algorithm is O(n·4096) time and uses the same amount of
    memory, which keeps it far below the 2‑second limit for n up to 10 000.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define COLS 6
#define MASKS (1 << COLS)           /* 64 */
#define STATES (MASKS * MASKS)      /* 4096 */

/* transition tables   (prev,curr) → list of valid next rows */
static uint8_t mid_next_list[STATES][MASKS];
static size_t  mid_next_len [STATES];

/* first row transitions   row0 → list of valid row1 rows */
static uint8_t first_next_list[MASKS][MASKS];
static size_t  first_next_len [MASKS];

/* final row validation  (prev,curr) → bool */
static uint8_t final_valid[STATES];

/* ------------------------------------------------------------------ */
/*  Validity test for a single row */
static inline bool valid_row(uint8_t prev, bool has_prev,
                             uint8_t mid, uint8_t next, bool has_next)
{
    for (int c = 0; c < COLS; ++c) {
        int bit = (mid >> c) & 1;
        int same = 0;

        /* above row */
        if (has_prev) {
            if (c > 0 && ((prev >> (c-1)) & 1) == bit)  ++same;
            if (((prev >> c) & 1) == bit)              ++same;
            if (c < COLS-1 && ((prev >> (c+1)) & 1) == bit) ++same;
        }

        /* same row left/right */
        if (c > 0 && ((mid >> (c-1)) & 1) == bit) ++same;
        if (c < COLS-1 && ((mid >> (c+1)) & 1) == bit) ++same;

        /* below row */
        if (has_next) {
            if (c > 0 && ((next >> (c-1)) & 1) == bit) ++same;
            if (((next >> c) & 1) == bit)             ++same;
            if (c < COLS-1 && ((next >> (c+1)) & 1) == bit) ++same;
        }

        /* disallowed counts are exactly 2 or 8 */
        if (same == 2 || same == 8) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Build all transition tables once */
static void build_transitions(void)
{
    /* middle rows transitions */
    for (int p = 0; p < MASKS; ++p) {
        for (int c = 0; c < MASKS; ++c) {
            size_t idx = (size_t)p * MASKS + c;
            size_t len = 0;
            for (int n = 0; n < MASKS; ++n) {
                if (valid_row((uint8_t)p, true,
                              (uint8_t)c, (uint8_t)n, true)) {
                    mid_next_list[idx][len++] = (uint8_t)n;
                }
            }
            mid_next_len[idx] = len;
            final_valid[idx] = valid_row((uint8_t)p, true,
                                         (uint8_t)c, 0, false);
        }
    }

    /* first row transitions (no above neighbors) */
    for (int c = 0; c < MASKS; ++c) {
        size_t len = 0;
        for (int n = 0; n < MASKS; ++n) {
            if (valid_row(0, false,
                          (uint8_t)c, (uint8_t)n, true)) {
                first_next_list[c][len++] = (uint8_t)n;
            }
        }
        first_next_len[c] = len;
    }
}

/* ------------------------------------------------------------------ */
/*  DP counting function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined from index 1 */

    /* lazily build tables */
    static bool built = false;
    if (!built) {
        build_transitions();
        built = true;
    }

    /* DP arrays for (prev,curr) states */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];
    for (size_t i = 0; i < STATES; ++i) cur[i] = 0;

    /* handle the trivial n = 1 case */
    if (n == 1) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < MASKS; ++r0) {
            if ((r0 & 1) == 0) { /* upper-left zero */
                if (valid_row(0, false, (uint8_t)r0, 0, false))
                    ans += (seq_t)1;
            }
        }
        return ans;
    }

    /* Initialise with all valid (row0,row1) pairs */
    for (int r0 = 0; r0 < MASKS; ++r0) {
        if ((r0 & 1) != 0) continue;            /* upper-left must be 0 */
        size_t len = first_next_len[r0];
        const uint8_t *list = first_next_list[r0];
        for (size_t k = 0; k < len; ++k) {
            int r1 = list[k];
            size_t idx = (size_t)r0 * MASKS + r1;
            cur[idx] += (seq_t)1;
        }
    }

    /* Process rows 2 .. n-1 (i.e., add the remaining rows) */
    for (idx_t row = 2; row < n; ++row) {
        for (size_t i = 0; i < STATES; ++i) nxt[i] = 0;
        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = cur[idx];
            if (val == 0) continue;
            uint8_t curr = (uint8_t)(idx % MASKS);
            size_t len = mid_next_len[idx];
            const uint8_t *list = mid_next_list[idx];
            for (size_t j = 0; j < len; ++j) {
                uint8_t next = list[j];
                size_t nidx = (size_t)curr * MASKS + next;
                nxt[nidx] += val;
            }
        }
        /* swap cur and nxt */
        for (size_t i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* Sum over states whose bottom row is valid */
    seq_t ans = 0;
    for (size_t idx = 0; idx < STATES; ++idx) {
        if (final_valid[idx]) ans += cur[idx];
    }
    return ans;
}
