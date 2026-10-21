/*
 * A243728  Number of length n+2 0..9 arrays with no three unequal elements in
 *          a row and no three equal elements in a row and new values 0..9
 *          introduced in 0..9 order.
 * https://oeis.org/A243728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 arrays over digits 0..9 in which * -
 *            no three equal elements appear consecutively, * - no three
 *            unequal elements appear consecutively, * - digits are introduced
 *            in increasing order (0 before 1, 1 before 2, …). * The sequence
 *            starts with a(1)=3, a(2)=6, a(3)=12, … (see...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 43, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n)
{
    /* The offset of the OEIS entry is 1, therefore a(1) is the first term. */
    if (n == 0) return 0;          /* Undefined for n < 1 */
    idx_t L = n + 2;               /* Length of the arrays */

    /* dp[last][second][max] holds the number of arrays of current length
     * whose last element is 'last', second‑last element is 'second',
     * and the largest digit used so far is 'max' (0..9). */
    static seq_t dp[10][10][10];
    static seq_t dp_next[10][10][10];
    memset(dp, 0, sizeof(dp));

    /* Initial arrays of length 2.  The first element is always 0. */
    dp[0][0][0] = 1;          /* 0,0 */
    dp[1][0][1] = 1;          /* 0,1 (new digit 1 introduced) */

    /* Extend the arrays to the required length L one element at a time. */
    for (idx_t len = 2; len < L; ++len)
    {
        memset(dp_next, 0, sizeof(dp_next));

        for (int last = 0; last < 10; ++last)
        {
            for (int second = 0; second < 10; ++second)
            {
                for (int m = 0; m < 10; ++m)
                {
                    seq_t cnt = dp[last][second][m];
                    if (!cnt) continue;

                    if (second == last)          /* last two equal */
                    {
                        /* The next digit cannot equal 'last'. */
                        for (int next = 0; next < 10; ++next)
                        {
                            if (next == last) continue;
                            if (next > m + 1) continue;   /* cannot skip a digit */
                            int newm = (next > m) ? next : m;
                            dp_next[next][last][newm] += cnt;
                        }
                    }
                    else                        /* last two distinct */
                    {
                        /* The next digit must be either 'last' or 'second'. */
                        dp_next[second][last][m] += cnt;     /* choose 'second' */
                        dp_next[last][last][m]   += cnt;     /* choose 'last'   */
                    }
                }
            }
        }

        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Sum all states to obtain the result. */
    seq_t total = 0;
    for (int last = 0; last < 10; ++last)
        for (int second = 0; second < 10; ++second)
            for (int m = 0; m < 10; ++m)
                total += dp[last][second][m];

    return total;
}
