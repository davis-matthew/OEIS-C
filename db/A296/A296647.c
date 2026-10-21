/*
 * A296647  Number of nX4 0..1 arrays with each 1 adjacent to 0, 1 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‑programming over rows for sequence A296647:
   number of n×4 binary arrays with each 1 adjacent to 0, 1, or 3 king‑move
   neighboring 1s.  (DP over pairs of consecutive rows.) */
#include <stdbool.h>

static bool ready = false;
static bool valid[16][16][16];
static bool last_ok[16][16];

static inline bool row_ok(uint8_t row, uint8_t up, uint8_t down)
{
    for (int c = 0; c < 4; ++c)
    {
        if (((row >> c) & 1) == 0)
            continue;
        int cnt = 0;
        /* up neighbors */
        if (c > 0 && ((up >> (c-1)) & 1))
            ++cnt;                      /* up-left */
        if (((up >> c) & 1))
            ++cnt;                      /* up */
        if (c < 3 && ((up >> (c+1)) & 1))
            ++cnt;                      /* up-right */
        /* same row neighbors */
        if (c > 0 && ((row >> (c-1)) & 1))
            ++cnt;                      /* left */
        if (c < 3 && ((row >> (c+1)) & 1))
            ++cnt;                      /* right */
        /* down neighbors */
        if (c > 0 && ((down >> (c-1)) & 1))
            ++cnt;                      /* down-left */
        if (((down >> c) & 1))
            ++cnt;                      /* down */
        if (c < 3 && ((down >> (c+1)) & 1))
            ++cnt;                      /* down-right */
        if (!(cnt == 0 || cnt == 1 || cnt == 3))
            return false;
    }
    return true;
}

static void init_transitions(void)
{
    for (int p1 = 0; p1 < 16; ++p1)
    {
        for (int p0 = 0; p0 < 16; ++p0)
        {
            for (int cur = 0; cur < 16; ++cur)
                valid[p1][p0][cur] = row_ok((uint8_t)p0, (uint8_t)p1, (uint8_t)cur);
            last_ok[p1][p0] = row_ok((uint8_t)p0, (uint8_t)p1, 0);
        }
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready)
        init_transitions();

    if (n == 0)
        return (seq_t)0;          /* offset is 1; a(0) is unused */

    seq_t dp[16][16]   = {{0}};
    seq_t next[16][16] = {{0}};

    /* initial state: previous‑previous row is 0 (empty), previous row is any pattern */
    for (int x = 0; x < 16; ++x)
        dp[0][x] = (seq_t)1;

    for (idx_t row = 2; row <= n; ++row)
    {
        /* clear next array */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                next[i][j] = (seq_t)0;

        for (int p1 = 0; p1 < 16; ++p1)
        {
            for (int p0 = 0; p0 < 16; ++p0)
            {
                seq_t curcnt = dp[p1][p0];
                if (curcnt == (seq_t)0)
                    continue;
                for (int cur = 0; cur < 16; ++cur)
                {
                    if (valid[p1][p0][cur])
                        next[p0][cur] = next[p0][cur] + curcnt;
                }
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum over final row validity (down = 0) */
    seq_t ans = (seq_t)0;
    for (int p1 = 0; p1 < 16; ++p1)
    {
        for (int p0 = 0; p0 < 16; ++p0)
        {
            if (last_ok[p1][p0])
                ans = ans + dp[p1][p0];
        }
    }
    return ans;
}
