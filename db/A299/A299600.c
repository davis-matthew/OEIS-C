/*
 * A299600  Number of nX6 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          7 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computation of OEIS sequence A299600 Number of n×6 0..1 arrays
 *            with every element equal to 2, 3, 4, 5, 7 or 8 king‑move
 *            adjacent elements, with upper left element zero
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* states: pair of rows (prevPrev, prev)  -> index = prevPrev*64 + prev */
#define ROWS 64
#define STATES (ROWS*ROWS)
#define MAX_TRANS 64   /* maximum transitions per state */

/* precomputed data */
static bool start_pair[STATES];              /* pair starts with row0 valid (bits) */
static bool final_pair[STATES];              /* pair ends with last row valid */
static uint8_t trans_count[STATES];
static uint16_t trans_from[STATES][MAX_TRANS];

/* flag whether precomputation was done */
static bool initialized = false;

/* allowed counts for a cell */
static const bool allow[9] = {0,0,1,1,1,1,0,1,1};

/* --------------------------- helper --------------------------- */
static inline bool row_valid(int mid, int upper, int lower)
/* check that every cell in row "mid" satisfies the adjacency rule
   with the given upper and lower rows.
   upper or lower may be -1 to denote absence. */
{
    for (int j = 0; j < 6; ++j) {
        int val = (mid >> j) & 1;
        int cnt = 0;

        /* same row */
        if (j > 0   && ((mid >> (j-1)) & 1) == val) ++cnt;
        if (j < 5   && ((mid >> (j+1)) & 1) == val) ++cnt;

        /* upper row */
        if (upper >= 0) {
            if (((upper >> j)      & 1) == val) ++cnt;          /* directly above */
            if (j > 0   && ((upper >> (j-1)) & 1) == val) ++cnt;   /* upper left */
            if (j < 5   && ((upper >> (j+1)) & 1) == val) ++cnt;   /* upper right */
        }

        /* lower row */
        if (lower >= 0) {
            if (((lower >> j)      & 1) == val) ++cnt;          /* directly below */
            if (j > 0   && ((lower >> (j-1)) & 1) == val) ++cnt;   /* lower left */
            if (j < 5   && ((lower >> (j+1)) & 1) == val) ++cnt;   /* lower right */
        }

        /* check allowed count */
        if (!allow[cnt])
            return false;
    }
    return true;
}

/* --------------------------- initialization --------------------------- */
static void precompute(void)
{
    /* start_pair: first row must satisfy its requirement with row1 */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            bool ok = ((r0 & 1) == 0) && row_valid(r0, -1, r1);
            start_pair[r0*ROWS + r1] = ok;
        }
    }

    /* transition lists and final_pair */
    for (int prevPrev = 0; prevPrev < ROWS; ++prevPrev) {
        for (int prev = 0; prev < ROWS; ++prev) {
            int s = prevPrev*ROWS + prev;
            uint8_t cnt = 0;
            for (int cur = 0; cur < ROWS; ++cur) {
                if (row_valid(prev, prevPrev, cur)) {
                    trans_from[s][cnt++] = prev*ROWS + cur;
                }
            }
            trans_count[s] = cnt;
        }
    }

    /* final_pair: last row must satisfy with previous row, no lower */
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            int s = prev*ROWS + cur;
            final_pair[s] = row_valid(cur, prev, -1);
        }
    }

    initialized = true;
}

/* --------------------------- DP arrays --------------------------- */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* --------------------------- main function --------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) precompute();

    /* n is 1‑based index */
    if (n == 0) return 0;           /* undefined, safe fallback */

    /* handle n == 1 */
    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < ROWS; ++r) {
            if (((r & 1) == 0) && row_valid(r, -1, -1))
                res += 1;
        }
        return res;
    }

    /* handle n == 2 */
    if (n == 2) {
        seq_t res = 0;
        for (int idx = 0; idx < STATES; ++idx) {
            if (start_pair[idx] && final_pair[idx])
                res += 1;
        }
        return res;
    }

    /* n >= 3: DP over rows */
    /* initialize dp_cur from start_pair */
    for (int s = 0; s < STATES; ++s)
        dp_cur[s] = start_pair[s] ? 1 : 0;

    /* iterate rows 2 .. n-1 (total n-2 transitions) */
    for (idx_t step = 2; step < n; ++step) {
        /* zero next array */
        for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            uint8_t cnt = trans_count[s];
            for (uint8_t t = 0; t < cnt; ++t) {
                int ns = trans_from[s][t];
                dp_next[ns] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (int s = 0; s < STATES; ++s) dp_cur[s] = dp_next[s];
    }

    /* sum results for final row validity */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s) {
        if (final_pair[s]) ans += dp_cur[s];
    }
    return ans;
}
