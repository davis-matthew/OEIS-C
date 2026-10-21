/*
 * A297097  Number of nX3 0..1 arrays with no 1 adjacent to 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A297097
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

#include <stdbool.h>
#include <stdint.h>

/* Sequence: Number of n×3 0..1 arrays with no 1 adjacent to 4 king‑move neighboring 1s.
   Computed with a transfer‑matrix DP over row patterns. */

static bool check_row(uint8_t top, uint8_t mid, uint8_t bot)
{
    for (int col = 0; col < 3; ++col)
    {
        if (!((mid >> col) & 1U))
            continue;  /* only check cells that are 1 */

        int cnt = 0;
        /* top row neighbors */
        if (col > 0) cnt += (top >> (col - 1)) & 1U;
        cnt += (top >> col) & 1U;
        if (col < 2) cnt += (top >> (col + 1)) & 1U;
        /* same row neighbors */
        if (col > 0) cnt += (mid >> (col - 1)) & 1U;
        if (col < 2) cnt += (mid >> (col + 1)) & 1U;
        /* bottom row neighbors */
        if (col > 0) cnt += (bot >> (col - 1)) & 1U;
        cnt += (bot >> col) & 1U;
        if (col < 2) cnt += (bot >> (col + 1)) & 1U;

        if (cnt == 4)
            return false;  /* forbidden configuration */
    }
    return true;
}

static bool allowed[8][8][8];
static bool allowed_inited = false;

static void init_allowed(void)
{
    if (allowed_inited)
        return;
    for (int top = 0; top < 8; ++top)
        for (int mid = 0; mid < 8; ++mid)
            for (int bot = 0; bot < 8; ++bot)
                allowed[top][mid][bot] = check_row((uint8_t)top,
                                                    (uint8_t)mid,
                                                    (uint8_t)bot);
    allowed_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return (seq_t)8;  /* all 2^3 patterns are valid for a single row */

    init_allowed();

    /* dp[prev][curr] – number of ways to reach the current state with
       the last two rows being (prev, curr). */
    seq_t dp[8][8] = {0};
    seq_t ndp[8][8] = {0};

    /* initial step: choose first two rows, ensuring the first row passes
       its constraint with a fictitious top row of zeros. */
    for (int r1 = 0; r1 < 8; ++r1)
        for (int r2 = 0; r2 < 8; ++r2)
            if (allowed[0][r1][r2])
                dp[r1][r2] = 1;

    if (n == 2)
    {
        seq_t total = 0;
        for (int r1 = 0; r1 < 8; ++r1)
            for (int r2 = 0; r2 < 8; ++r2)
                if (allowed[r1][r2][0]) /* last row with bottom zeros */
                    total += dp[r1][r2];
        return total;
    }

    /* process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row)
    {
        /* clear ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev)
        {
            for (int curr = 0; curr < 8; ++curr)
            {
                seq_t cnt = dp[prev][curr];
                if (cnt == 0)
                    continue;
                for (int nxt = 0; nxt < 8; ++nxt)
                {
                    if (allowed[prev][curr][nxt])
                        ndp[curr][nxt] += cnt;
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row check with bottom row of zeros */
    seq_t total = 0;
    for (int prev = 0; prev < 8; ++prev)
        for (int curr = 0; curr < 8; ++curr)
            if (allowed[prev][curr][0])
                total += dp[prev][curr];

    return total;
}
