/*
 * A295601  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2, 3 or 4 1s.
 * https://oeis.org/A295601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming over 3‑bit row patterns. */

#include <stdbool.h>
#include <string.h>

static bool _precomp_done = false;
static int _horiz[8][3];
static bool _ok[8][8][8];
static bool _first[8][8];
static bool _last[8][8];
static bool _none[8];

/* Test whether a row `cur` is valid given the patterns of its top and
   bottom neighbours.  `top_defined` / `bottom_defined` indicate whether
   those neighbours exist (i.e., are not at the boundary). */
static bool _row_valid(uint8_t cur, uint8_t top, uint8_t bottom,
                       bool top_defined, bool bottom_defined)
{
    for (int j = 0; j < 3; ++j) {
        if ((cur >> j) & 1) {
            int adj = _horiz[cur][j];
            if (top_defined)   adj += (top >> j) & 1;
            if (bottom_defined)adj += (bottom >> j) & 1;
            if (adj == 1) return false;   /* forbidden adjacency count */
        }
    }
    return true;
}

/* Performs all one‑time precomputation of helper tables. */
static void _precompute(void)
{
    /* horizontal adjacency counts for each column in a row */
    for (int r = 0; r < 8; ++r) {
        for (int j = 0; j < 3; ++j) {
            int cnt = 0;
            if (j > 0   && ((r >> (j - 1)) & 1)) ++cnt;
            if (j < 2   && ((r >> (j + 1)) & 1)) ++cnt;
            _horiz[r][j] = cnt;
        }
    }

    /* precompute tables for all combinations of neighbours */
    for (int p = 0; p < 8; ++p)
        for (int r = 0; r < 8; ++r)
            for (int b = 0; b < 8; ++b)
                _ok[p][r][b] = _row_valid((uint8_t)r, (uint8_t)p,
                                          (uint8_t)b, true, true);

    for (int b = 0; b < 8; ++b)
        for (int r = 0; r < 8; ++r)
            _first[b][r] = _row_valid((uint8_t)r, 0, (uint8_t)b, false, true);

    for (int p = 0; p < 8; ++p)
        for (int r = 0; r < 8; ++r)
            _last[p][r] = _row_valid((uint8_t)r, (uint8_t)p, 0, true, false);

    for (int r = 0; r < 8; ++r)
        _none[r] = _row_valid((uint8_t)r, 0, 0, false, false);

    _precomp_done = true;
}

seq_t a(idx_t n)
{
    if (!_precomp_done) _precompute();

    if (n == 0) return 0;
    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < 8; ++r)
            if (_none[r]) res += 1;
        return res;
    }

    /* DP arrays: dp_prev[p][c] = number of ways to reach a state where
       the last two rows are (p, c).  All rows up to row `p` have already
       satisfied their constraints. */
    static seq_t dp_prev[8][8], dp_next[8][8];
    memset(dp_prev, 0, sizeof(dp_prev));
    memset(dp_next, 0, sizeof(dp_next));

    /* initialise for first two rows */
    for (int p = 0; p < 8; ++p)
        for (int c = 0; c < 8; ++c)
            if (_first[c][p]) dp_prev[p][c] = 1;

    /* process rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int p = 0; p < 8; ++p) {
            for (int c = 0; c < 8; ++c) {
                seq_t cur = dp_prev[p][c];
                if (cur == 0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (_ok[p][c][nxt])
                        dp_next[c][nxt] += cur;
                }
            }
        }
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
    }

    /* final column: last row must have no bottom neighbour */
    seq_t result = 0;
    for (int p = 0; p < 8; ++p) {
        for (int c = 0; c < 8; ++c) {
            if (dp_prev[p][c] != 0 && _last[p][c])
                result += dp_prev[p][c];
        }
    }
    return result;
}
