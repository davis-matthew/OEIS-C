/*
 * A283853  Number of nX4 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283853
   Number of n×4 binary arrays with no 1 equal to more than three of its
   horizontal and diagonal (upper‑left, upper‑right, lower‑left, lower‑right)
   neighbours.  The vertical neighbours are not counted.  The function
   uses a dynamic‑programming over rows with a pre‑computed truth table
   that tells whether a middle row satisfies the neighbour condition
   given the rows above and below.  Complexity O(n·16³). */

#include <stdbool.h>

static bool ok[16][16][16];
static bool ok_initialized = false;

static void init_ok(void)
{
    if (ok_initialized) return;
    for (int prev = 0; prev < 16; ++prev)
        for (int cur  = 0; cur  < 16; ++cur)
            for (int next = 0; next < 16; ++next)
            {
                bool valid = true;
                for (int c = 0; c < 4; ++c)
                {
                    if (!((cur >> c) & 1)) continue;
                    int cnt = 0;
                    /* horizontal neighbours */
                    if (c > 0   && ((cur >> (c-1)) & 1)) ++cnt;
                    if (c < 3   && ((cur >> (c+1)) & 1)) ++cnt;
                    /* upper diagonals */
                    if (c > 0   && ((prev >> (c-1)) & 1)) ++cnt;
                    if (c < 3   && ((prev >> (c+1)) & 1)) ++cnt;
                    /* lower diagonals */
                    if (c > 0   && ((next >> (c-1)) & 1)) ++cnt;
                    if (c < 3   && ((next >> (c+1)) & 1)) ++cnt;
                    if (cnt > 3) { valid = false; break; }
                }
                ok[prev][cur][next] = valid;
            }
    ok_initialized = true;
}

seq_t a(idx_t n)
{
    init_ok();

    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    /* initialise dp for empty prefix */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;          /* sentinel zero rows before the first real row */

    for (idx_t r = 0; r < n; ++r)
    {
        /* clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prevprev = 0; prevprev < 16; ++prevprev)
            for (int prev = 0; prev < 16; ++prev)
            {
                seq_t val = dp[prevprev][prev];
                if (!val) continue;
                for (int cur = 0; cur < 16; ++cur)
                    if (ok[prevprev][prev][cur])
                        ndp[prev][cur] += val;
            }

        /* swap dp and ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* handle the last row by checking it against a trailing zero row */
    seq_t result = 0;
    for (int prevprev = 0; prevprev < 16; ++prevprev)
        for (int prev = 0; prev < 16; ++prev)
        {
            seq_t val = dp[prevprev][prev];
            if (!val) continue;
            if (ok[prevprev][prev][0])   /* zero sentinel below */
                result += val;
        }

    return result;
}
