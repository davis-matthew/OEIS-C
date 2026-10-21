/*
 * A299312  Number of nX6 0..1 arrays with every element equal to 1, 2, 4, 5,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   _check_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299312
    Number of n×6 binary arrays with king‑move adjacency constraints
    (each cell must have 1,2,4,5,6 or 7 adjacent 1‑cells),
    upper‑left cell forced to 0.
    This implementation uses a transfer‑matrix dynamic programming
    over all 64 possible 6‑bit row patterns. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/_check_row.h"

/* ---------- tables (built once) ---------- */
static unsigned char  _next_list[64][64][64];
static unsigned char  _next_count[64][64];
static unsigned char  _top[64][64];
static unsigned char  _bottom[64][64];
static int            _built = 0;

static void _build_tables(void)
{
    for (int up = 0; up < 64; ++up)
    {
        for (int mid = 0; mid < 64; ++mid)
        {
            /* top: no upper neighbour, down = up */
            _top[mid][up] = _check_row(0, (uint8_t)mid, (uint8_t)up, 0, 1);

            /* bottom: no lower neighbour, up = up */
            _bottom[up][mid] = _check_row((uint8_t)up, (uint8_t)mid, 0, 1, 0);

            /* internal transitions */
            unsigned char k = 0;
            for (int down = 0; down < 64; ++down)
            {
                if (_check_row((uint8_t)up, (uint8_t)mid, (uint8_t)down, 1, 1))
                    _next_list[up][mid][k++] = (uint8_t)down;
            }
            _next_count[up][mid] = k;
        }
    }
    _built = 1;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!_built) _build_tables();

    if (n == 0) return 0;          /* out of offset, unused */

    /* ---------- n == 1 : single row ---------- */
    if (n == 1)
    {
        seq_t ans = 0;
        for (int mask = 0; mask < 64; ++mask)
        {
            if ((mask & 1) == 0 && _check_row(0, (uint8_t)mask, 0, 0, 0))
                ans += (seq_t)1;
        }
        return ans;
    }

    /* ---------- DP arrays (size 64*64) ---------- */
    static seq_t dp[64*64];
    static seq_t nxt[64*64];
    const size_t SZ = 64*64;

    /* zero initialise */
    for (size_t i = 0; i < SZ; ++i) dp[i] = 0;

    /* initial two rows (row1 is the top row) */
    for (int row1 = 0; row1 < 64; ++row1)
    {
        if ((row1 & 1) == 0)          /* upper-left must be 0 */
        {
            for (int row2 = 0; row2 < 64; ++row2)
            {
                if (_top[row1][row2])
                    dp[(row1 << 6) | row2] += (seq_t)1;
            }
        }
    }

    /* process rows 3 .. n */
    for (idx_t r = 3; r <= n; ++r)
    {
        for (size_t i = 0; i < SZ; ++i) nxt[i] = 0;

        for (int prev = 0; prev < 64; ++prev)
        {
            for (int curr = 0; curr < 64; ++curr)
            {
                seq_t val = dp[(prev << 6) | curr];
                if (val == 0) continue;

                unsigned char k = _next_count[prev][curr];
                for (unsigned char t = 0; t < k; ++t)
                {
                    int next = _next_list[prev][curr][t];
                    nxt[(curr << 6) | next] += val;
                }
            }
        }
        /* copy nxt -> dp for next iteration */
        for (size_t i = 0; i < SZ; ++i) dp[i] = nxt[i];
    }

    /* final bottom row constraint */
    seq_t ans = 0;
    for (int prev = 0; prev < 64; ++prev)
        for (int curr = 0; curr < 64; ++curr)
        {
            seq_t val = dp[(prev << 6) | curr];
            if (val == 0) continue;
            if (_bottom[prev][curr]) ans += val;
        }

    return ans;
}
