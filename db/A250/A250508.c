/*
 * A250508  Number of (n+2)X(1+2) 0..2 arrays with nondecreasing maximum minus
 *          minimum of every three consecutive values in every row and column.
 * https://oeis.org/A250508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of length (n+2) 0..2 column sequences with
 *            nondecreasing max–min over every three consecutive values)³
 *            [i.e. the cube of the number of admissible column sequences]
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline int min3(int a, int b, int c)
{
    int m = a < b ? a : b;
    return c < m ? c : m;
}

static inline int max3(int a, int b, int c)
{
    int m = a > b ? a : b;
    return c > m ? c : m;
}

seq_t a(idx_t n)
{
    idx_t L = n + 2;                 /* length of the column */

    /* state = (a1,a2,a3,d) encoded as (((a1*3)+a2)*3+a3)*3+d */
    #define STATES 81               /* 3³ * 3 */
    seq_t dp[STATES];
    seq_t next[STATES];
    size_t i;

    /* initialise dp for all 3‑tuples (length 3) */
    for (i = 0; i < STATES; ++i) dp[i] = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                int d = max3(a, b, c) - min3(a, b, c);   /* 0..2 */
                size_t idx = (((a * 3 + b) * 3 + c) * 3 + d);
                dp[idx] = dp[idx] + 1;
            }

    /* extend the sequence for positions 4 … L */
    for (idx_t pos = 3; pos < L; ++pos) {
        for (i = 0; i < STATES; ++i) next[i] = 0;
        for (i = 0; i < STATES; ++i) {
            seq_t ways = dp[i];
            if (!ways) continue;

            /* decode state */
            int d_prev = i % 3;
            int tmp = i / 3;
            int a3 = tmp % 3;                 /* last element of triple */
            int a2 = (tmp / 3) % 3;           /* middle element      */
            int a1 = (tmp / 9) % 3;           /* first element       */

            /* try each possible new element x at the next position */
            for (int x = 0; x < 3; ++x) {
                int d_new = max3(a2, a3, x) - min3(a2, a3, x);
                if (d_new < d_prev) continue;  /* must be nondecreasing */

                size_t idx2 = ((((a2 * 3) + a3) * 3 + x) * 3 + d_new);
                next[idx2] = next[idx2] + ways;
            }
        }
        for (i = 0; i < STATES; ++i) dp[i] = next[i];
    }

    /* sum all final states to get the number of valid column sequences */
    seq_t total = 0;
    for (i = 0; i < STATES; ++i) total = total + dp[i];

    /* a(n) is the cube of this number */
    seq_t temp = total * total;
    return temp * total;
}
