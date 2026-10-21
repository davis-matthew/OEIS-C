/*
 * A243606  Number of length n+2 0..9 arrays with no three elements in a row
 *          with pattern aba (with a!=b) and new values 0..9 introduced in
 *          0..9 order.
 * https://oeis.org/A243606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of length n+2 arrays over digits 0..9 that (1) introduce
 *            new digits in increasing order (restricted growth function
 *            property) and (2) contain no three consecutive elements of form
 *            a b a with a!=b. The number of such arrays for a given n is
 *            denoted a(n). The computation uses a dynamic progra...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1.  For safety, treat n=0 as 0. */
    if (n == 0)
        return 0;

    /* Length of the arrays to consider: L = n+2. */
    idx_t L = n + 2;

    /* DP arrays: dp[2][m][p][q] holds counts for a given length.
       m = current maximum digit (0..9),
       p and q are the last two digits (≤ m). */
    static seq_t dp[2][10][10][10];
    int prev = 0, next = 1;

    /* Initialize all cells to zero. */
    for (int i = 0; i < 2; ++i)
        for (int m = 0; m < 10; ++m)
            for (int p = 0; p <= m; ++p)
                for (int q = 0; q <= m; ++q)
                    dp[i][m][p][q] = 0;

    /* Initial sequences of length 2: [0,0] and [0,1]. */
    dp[prev][0][0][0] = 1;   /* m = 0, last two digits 0,0 */
    dp[prev][1][0][1] = 1;   /* m = 1, last two digits 0,1 */

    /* Iterate from length 2 to L-1 (each step adds one digit). */
    for (idx_t len = 2; len < L; ++len)
    {
        /* Zero the next layer. */
        for (int m = 0; m < 10; ++m)
            for (int p = 0; p <= m; ++p)
                for (int q = 0; q <= m; ++q)
                    dp[next][m][p][q] = 0;

        /* Transition from prev layer to next layer. */
        for (int m = 0; m < 10; ++m)
        {
            for (int p = 0; p <= m; ++p)
            {
                for (int q = 0; q <= m; ++q)
                {
                    seq_t cnt = dp[prev][m][p][q];
                    if (cnt == 0)
                        continue;

                    /* Possible next digits r: 0 .. m+1 (but ≤9). */
                    for (int r = 0; r <= m + 1 && r <= 9; ++r)
                    {
                        /* Forbidden pattern: if p != q and r == p. */
                        if (p != q && r == p)
                            continue;

                        int new_m = (r > m) ? r : m;
                        dp[next][new_m][q][r] += cnt;
                    }
                }
            }
        }

        /* Swap layers for next iteration. */
        int tmp = prev;
        prev = next;
        next = tmp;
    }

    /* Sum all counts for the final length L. */
    seq_t total = 0;
    for (int m = 0; m < 10; ++m)
        for (int p = 0; p <= m; ++p)
            for (int q = 0; q <= m; ++q)
                total += dp[prev][m][p][q];

    return total;
}
