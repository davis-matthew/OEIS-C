/*
 * A241613  Number of length n+2 0..7 arrays with no consecutive three
 *          elements summing to more than 7.
 * https://oeis.org/A241613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 arrays over 0..7 with no three
 *            consecutive elements summing to more than 7. The sequence is
 *            computed with a simple DP over the last two elements of the
 *            array
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

/* Function supplied by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* L = n + 2 is the length of the array */
    /* Number of steps needed to build the array from
       an initial pair (a0, a1) to length L */
    idx_t steps = n;   /* L - 2 = n */

    /* DP tables holding counts for each pair (x, y) */
    seq_t dp[8][8] = {0};
    seq_t ndp[8][8];

    /* Every pair (x, y) is initially one possible prefix
       of length 2 */
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            dp[x][y] = (seq_t)1;

    /* Perform the DP for 'steps' transitions */
    for (idx_t step = 0; step < steps; ++step)
    {
        /* zero ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = (seq_t)0;

        for (int x = 0; x < 8; ++x)
        {
            for (int y = 0; y < 8; ++y)
            {
                seq_t cnt = dp[x][y];
                if (!cnt) continue;

                int sumxy = x + y;
                for (int z = 0; z < 8; ++z)
                {
                    if (sumxy + z <= 7)
                        ndp[y][z] += cnt;
                }
            }
        }

        /* copy ndp back to dp for next step */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all counts for the final DP table */
    seq_t total = (seq_t)0;
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            total += dp[x][y];

    return total;
}
