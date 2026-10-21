/*
 * A296403  Number of nX6 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A296403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: compute the number of n×6 binary arrays in which every 1
 * has exactly 2 or 3 neighboring 1s among horizontally, vertically,
 * or anti‑diagonally adjacent cells.  The algorithm uses dynamic
 * programming over the last two rows of the array. */

#include <stdbool.h>
#include <stdint.h>

/* Validity test for a single row given the row above and the row below.
 * Each 1 in 'cur' must have 2 or 3 neighboring 1s among:
 *   horizontal (left, right), vertical (up, down), and
 *   anti‑diagonal (up‑right, down‑left). */
static inline bool valid_row(idx_t cur, idx_t above, idx_t below)
{
    for (int j = 0; j < 6; ++j) {
        if ((cur >> j) & 1) {
            int cnt = 0;
            /* horizontal neighbours */
            if (j > 0      && (cur >> (j - 1)) & 1) ++cnt;
            if (j < 5      && (cur >> (j + 1)) & 1) ++cnt;
            /* vertical neighbours */
            if (above & ((idx_t)1 << j)) ++cnt;
            if (below & ((idx_t)1 << j)) ++cnt;
            /* anti‑diagonal neighbours (up‑right, down‑left) */
            if (j < 5      && (above & ((idx_t)1 << (j + 1)))) ++cnt; /* up‑right */
            if (j > 0      && (below & ((idx_t)1 << (j - 1)))) ++cnt; /* down‑left */
            if (cnt != 2 && cnt != 3) return false;
        }
    }
    return true;
}

/* Transition table: nextMask[prev][cur] is a bitmask of all rows
 * that can follow (prev,cur) while keeping 'cur' valid. */
static uint64_t nextMask[64][64];
static bool transitionsReady = false;

static void precompute(void)
{
    if (transitionsReady) return;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            uint64_t mask = 0;
            for (int next = 0; next < 64; ++next) {
                if (valid_row((idx_t)cur, (idx_t)prev, (idx_t)next))
                    mask |= 1ULL << next;
            }
            nextMask[prev][cur] = mask;
        }
    }
    transitionsReady = true;
}

seq_t a(idx_t n)
{
    precompute();

    if (n == 0) return 0;                       /* safety for offset < 1 */

    /* --- Handle n == 1 ------------------------------------------------- */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t r0 = 0; r0 < 64; ++r0)
            if (valid_row(r0, 0, 0))
                res += (seq_t)1;
        return res;
    }

    /* --- DP tables ----------------------------------------------------- */
    seq_t dpPrev[64][64] = {0};
    seq_t dpNext[64][64] = {0};

    /* Initialise with the first two rows.  r0 must be valid with
     * above = 0 and below = r1. */
    for (int r0 = 0; r0 < 64; ++r0) {
        uint64_t mask = nextMask[0][r0];   /* rows allowed as r1 */
        for (int r1 = 0; r1 < 64; ++r1)
            if (mask & (1ULL << r1))
                dpPrev[r0][r1] = (seq_t)1;
    }

    /* Process rows 2 .. n-1 */
    for (idx_t row = 2; row < n; ++row) {
        /* clear dpNext */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dpNext[i][j] = 0;

        /* propagate to the next row */
        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                seq_t ways = dpPrev[prev][cur];
                if (!ways) continue;
                uint64_t mask = nextMask[prev][cur];
                for (int next = 0; next < 64; ++next)
                    if (mask & (1ULL << next))
                        dpNext[cur][next] += ways;
            }
        }

        /* swap tables */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }

    /* --- Final row check ------------------------------------------------ */
    seq_t result = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            seq_t ways = dpPrev[prev][cur];
            if (!ways) continue;
            /* check that the last row 'cur' is valid with below = 0 */
            if (nextMask[prev][cur] & 1ULL)
                result += ways;
        }
    }
    return result;
}
