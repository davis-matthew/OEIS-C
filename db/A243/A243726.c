/*
 * A243726  Number of length n+2 0..7 arrays with no three unequal elements in
 *          a row and no three equal elements in a row and new values 0..7
 *          introduced in 0..7 order.
 * https://oeis.org/A243726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 0..7 arrays with no three unequal
 *            elements in a row and no three equal elements in a row, where
 *            new values 0..7 are introduced in ascending order. This
 *            implementation uses dynamic programming over the last two
 *            symbols and the current maximum symbol used
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 43, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Array dimension constants */
    const int MAX_VAL = 8;        /* values 0..7 */
    const int MAX_M   = 8;        /* maximum symbol value used (0..7) */

    /* Length of the array to count: L = n + 2 */
    idx_t L = n + (idx_t)2;

    /* Current and next DP tables: [last1][last2][maxUsed] */
    seq_t cur[MAX_VAL][MAX_VAL][MAX_M];
    seq_t nxt[MAX_VAL][MAX_VAL][MAX_M];

    /* Zero initial tables */
    for (int i = 0; i < MAX_VAL; ++i)
        for (int j = 0; j < MAX_VAL; ++j)
            for (int k = 0; k < MAX_M; ++k)
                cur[i][j][k] = nxt[i][j][k] = 0;

    /* Base cases – length 2 */
    for (int x = 0; x < MAX_VAL; ++x)
    {
        /* Starting with 0 is required by the introduction order rule */
        if (x != 0) continue;
        cur[x][x][x] = 1;                     /* (0,0) */
        if (x < MAX_VAL - 1)                   /* (0,1) */
            cur[x][x + 1][x + 1] = 1;
    }

    /* Dynamic programming for lengths 2 .. L-1 */
    for (idx_t len = 2; len < L; ++len)
    {
        /* Zero nxt table for this iteration */
        for (int i = 0; i < MAX_VAL; ++i)
            for (int j = 0; j < MAX_VAL; ++j)
                for (int k = 0; k < MAX_M; ++k)
                    nxt[i][j][k] = 0;

        for (int a = 0; a < MAX_VAL; ++a)
        {
            for (int b = 0; b < MAX_VAL; ++b)
            {
                for (int m = 0; m < MAX_M; ++m)
                {
                    seq_t ways = cur[a][b][m];
                    if (ways == 0) continue;

                    /* Existing values 0..m */
                    for (int y = 0; y <= m; ++y)
                    {
                        /* Triple check */
                        int all_equal = (a == b && b == y);
                        int all_distinct = (a != b && a != y && b != y);
                        if (!all_equal && !all_distinct)
                            nxt[b][y][m] += ways;
                    }

                    /* Potential new value m+1 if within bounds */
                    if (m < 7)
                    {
                        int y = m + 1;          /* new value */
                        int all_equal = (a == b && b == y);
                        int all_distinct = (a != b && a != y && b != y);
                        if (!all_equal && !all_distinct)
                            nxt[b][y][y] += ways;  /* new maximum becomes y */
                    }
                }
            }
        }

        /* Copy next table to current for the next iteration */
        for (int i = 0; i < MAX_VAL; ++i)
            for (int j = 0; j < MAX_VAL; ++j)
                for (int k = 0; k < MAX_M; ++k)
                    cur[i][j][k] = nxt[i][j][k];
    }

    /* Sum all states of length L */
    seq_t result = 0;
    for (int a = 0; a < MAX_VAL; ++a)
        for (int b = 0; b < MAX_VAL; ++b)
            for (int m = 0; m < MAX_M; ++m)
                result += cur[a][b][m];

    return result;
}
