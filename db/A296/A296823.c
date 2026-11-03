/*
 * A296823  Number of nX4 0..1 arrays with each 1 adjacent to 2, 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which each 1 has 2, 3, or
 *            4 king‑move neighbouring 1s. The function computes the sequence
 *            by dynamic programming over columns
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 206 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t */

/* Test whether, given three consecutive columns (prev, curr, next),
   every 1 in curr satisfies the adjacency requirement.  */
static int _is_valid(int prev, int curr, int next)
{
    for (int r = 0; r < 4; ++r)
    {
        if ((curr & (1 << r)) == 0)
            continue;                 /* no 1 in this cell */

        int cnt = 0;

        /* neighbours from previous and next columns (diagonals and horizontals) */
        for (int dr = -1; dr <= 1; ++dr)
        {
            int rr = r + dr;
            if (rr >= 0 && rr < 4)
            {
                if (prev & (1 << rr)) ++cnt;
                if (next & (1 << rr)) ++cnt;
            }
        }

        /* vertical neighbours in the same column */
        if (r > 0     && (curr & (1 << (r - 1)))) ++cnt;
        if (r < 3     && (curr & (1 << (r + 1)))) ++cnt;

        if (cnt < 2 || cnt > 4)               /* must be 2,3 or 4 */
            return 0;
    }
    return 1;
}

static int _valid_inited = 0;
static int _valid[16][16][16];

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* initialise validity table once */
    if (!_valid_inited)
    {
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                for (int nx = 0; nx < 16; ++nx)
                    _valid[p][c][nx] = _is_valid(p, c, nx);
        _valid_inited = 1;
    }

    /* DP arrays: state encoded as (prev << 4) | curr  (16×16 = 256 states) */
    seq_t dp[256]  = {0};
    seq_t ndp[256] = {0};

    /* base: first column, with an implicit previous zero column */
    for (int first = 0; first < 16; ++first)
        dp[first] = 1;                 /* prev = 0, curr = first */

    /* process remaining columns */
    for (idx_t col = 2; col <= n; ++col)
    {
        for (size_t i = 0; i < 256; ++i)
            ndp[i] = 0;

        for (int prev = 0; prev < 16; ++prev)
        {
            for (int curr = 0; curr < 16; ++curr)
            {
                size_t idx = ((size_t)prev << 4) | curr;
                seq_t ways = dp[idx];
                if (ways == 0) continue;

                for (int next = 0; next < 16; ++next)
                    if (_valid[prev][curr][next])
                    {
                        size_t nidx = ((size_t)curr << 4) | next;
                        ndp[nidx] += ways;
                    }
            }
        }

        /* move to next column */
        for (size_t i = 0; i < 256; ++i)
            dp[i] = ndp[i];
    }

    /* finalize: the last column must be valid against a trailing zero column */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev)
    {
        for (int curr = 0; curr < 16; ++curr)
        {
            size_t idx = ((size_t)prev << 4) | curr;
            seq_t ways = dp[idx];
            if (ways == 0) continue;
            if (_valid[prev][curr][0])
                result += ways;
        }
    }
    return result;
}
