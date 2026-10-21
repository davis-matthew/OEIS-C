/*
 * A298068  Number of nX6 0..1 arrays with every element equal to 2, 3, 4 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays with upper‑left element 0, and
 *            for every entry the number of king‑move adjacent entries with
 *            the same value is in {2,3,4,6}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define COLS 6
#define ROWS (1u << COLS)   /* 64 */
#define NONE  ROWS           /* sentinel for "no row" */

/* Precomputed data ----------------------------------------------------- */

static uint64_t mask_internal[ROWS][ROWS];   /* bitmask of next rows */
static uint64_t mask_start[ROWS];            /* for first transition */
static bool     valid_end[ROWS][ROWS];       /* last row with no next */
static bool     valid_single[ROWS];          /* single row, no neighbours */

/* Helper: test validity of a triple (prev,curr,next).
   prev or next may be NONE */
static bool valid_triplet(int prev, int curr, int next)
{
    for (int c = 0; c < COLS; ++c) {
        int bit = (curr >> c) & 1;
        int cnt = 0;

        /* above neighbours */
        if (prev != NONE) {
            if (c > 0 && ((prev >> (c - 1)) & 1) == bit) ++cnt;
            if (((prev >> c) & 1) == bit) ++cnt;
            if (c < COLS - 1 && ((prev >> (c + 1)) & 1) == bit) ++cnt;
        }

        /* horizontal neighbours (in the same row) */
        if (c > 0 && ((curr >> (c - 1)) & 1) == bit) ++cnt;
        if (c < COLS - 1 && ((curr >> (c + 1)) & 1) == bit) ++cnt;

        /* below neighbours */
        if (next != NONE) {
            if (c > 0 && ((next >> (c - 1)) & 1) == bit) ++cnt;
            if (((next >> c) & 1) == bit) ++cnt;
            if (c < COLS - 1 && ((next >> (c + 1)) & 1) == bit) ++cnt;
        }

        if (cnt != 2 && cnt != 3 && cnt != 4 && cnt != 6)
            return false;
    }
    return true;
}

/* Build tables on first use */
static bool tables_ready = false;
static void build_tables(void)
{
    if (tables_ready) return;
    for (int curr = 0; curr < ROWS; ++curr) {
        uint64_t m = 0;
        for (int next = 0; next < ROWS; ++next)
            if (valid_triplet(NONE, curr, next))
                m |= (1ULL << next);
        mask_start[curr] = m;
    }
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            uint64_t m = 0;
            for (int next = 0; next < ROWS; ++next)
                if (valid_triplet(prev, curr, next))
                    m |= (1ULL << next);
            mask_internal[prev][curr] = m;
            valid_end[prev][curr] = valid_triplet(prev, curr, NONE);
        }
    }
    for (int curr = 0; curr < ROWS; ++curr)
        valid_single[curr] = valid_triplet(NONE, curr, NONE);
    tables_ready = true;
}

/* Main function -------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of bounds, unused */
    build_tables();

    /* Single row (n==1) */
    if (n == 1) {
        seq_t total = 0;
        for (int curr = 0; curr < ROWS; ++curr) {
            if ((curr & 1) == 0 && valid_single[curr])
                total += (seq_t)1;
        }
        return total;
    }

    /* DP arrays: dp[prev][curr] */
    static seq_t dp[ROWS + 1][ROWS];
    static seq_t ndp[ROWS + 1][ROWS];
    for (int p = 0; p <= ROWS; ++p)
        for (int c = 0; c < ROWS; ++c)
            dp[p][c] = ndp[p][c] = (seq_t)0;

    /* initial state – first row, prev absent */
    for (int curr = 0; curr < ROWS; ++curr) {
        if ((curr & 1) == 0)           /* upper-left must be 0 */
            dp[NONE][curr] = (seq_t)1;
    }

    /* iterate over all transitions: n-1 steps */
    for (idx_t step = 1; step < n; ++step) {
        for (int p = 0; p <= ROWS; ++p)
            for (int c = 0; c < ROWS; ++c)
                ndp[p][c] = (seq_t)0;

        for (int prev = 0; prev <= ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                uint64_t mask = (prev == NONE) ? mask_start[curr]
                                               : mask_internal[prev][curr];
                while (mask) {
                    uint64_t lsb = mask & -mask;
                    int next = __builtin_ctzll(mask);
                    ndp[curr][next] += ways;
                    mask ^= lsb;
                }
            }
        }
        /* swap dp and ndp */
        for (int p = 0; p <= ROWS; ++p)
            for (int c = 0; c < ROWS; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* final validation of last row */
    seq_t total = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            if (!dp[prev][curr]) continue;
            if (valid_end[prev][curr])
                total += dp[prev][curr];
        }
    }
    return total;
}
