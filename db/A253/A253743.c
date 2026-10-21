/*
 * A253743  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A253743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} whose every 2×2
 *            subblock satisfies (NE − SW anti‑diagonal difference) is
 *            non‑decreasing left to right, (NW + SE diagonal sum) is
 *            non‑decreasing top to bottom. The function uses a dynamic
 *            program over the last row and the sums of the previous two
 *            rows....
 * Range:     u8 none, u16 n <= 3, u32 n <= 13, u64 n <= 38, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define ROWS   27   /* all 3‑digit rows over {0,1,2} */
#define SUMS   5    /* possible sums 0..4 of two digits */
#define STATES (ROWS * SUMS * SUMS)

/* pre‑computed digit representation for each of the 27 rows */
static const unsigned char row_digits[ROWS][3] = {
    {0,0,0}, {0,0,1}, {0,0,2},
    {0,1,0}, {0,1,1}, {0,1,2},
    {0,2,0}, {0,2,1}, {0,2,2},
    {1,0,0}, {1,0,1}, {1,0,2},
    {1,1,0}, {1,1,1}, {1,1,2},
    {1,2,0}, {1,2,1}, {1,2,2},
    {2,0,0}, {2,0,1}, {2,0,2},
    {2,1,0}, {2,1,1}, {2,1,2},
    {2,2,0}, {2,2,1}, {2,2,2}
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behaviour for indices below the offset is irrelevant */

    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    idx_t i, j, k;

    /* initialise current table to zero */
    for (i = 0; i < STATES; ++i)
        dp_cur[i] = 0;

    /* initial pair of rows (rowA, rowB) must satisfy horizontal rule */
    for (i = 0; i < ROWS; ++i) {          /* rowA */
        for (j = 0; j < ROWS; ++j) {      /* rowB */
            int diff0 = (int)row_digits[i][1] - (int)row_digits[j][0];
            int diff1 = (int)row_digits[i][2] - (int)row_digits[j][1];
            if (diff0 > diff1)           /* horizontal non‑decreasing */
                continue;

            int sum0 = (int)row_digits[i][0] + (int)row_digits[j][1];
            int sum1 = (int)row_digits[i][1] + (int)row_digits[j][2];
            /* sums always lie in 0..4 for digits 0..2, but check for safety */
            if (sum0 < 0 || sum0 >= SUMS || sum1 < 0 || sum1 >= SUMS)
                continue;

            idx_t state = (j * SUMS + sum0) * SUMS + sum1;
            dp_cur[state] += (seq_t)1;
        }
    }

    /* If only two rows are needed (n == 1) */
    if (n == 1) {
        seq_t ans = 0;
        for (i = 0; i < STATES; ++i)
            ans += dp_cur[i];
        return ans;
    }

    /* Add remaining rows (total rows = n+1, we already used 2) */
    for (k = 1; k <= n - 1; ++k) {        /* k = 1 … n-1 */
        /* clear next table */
        for (i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        /* process every state */
        for (i = 0; i < STATES; ++i) {
            seq_t cnt = dp_cur[i];
            if (cnt == 0)
                continue;

            /* decode current state: (prevRow, prevSum0, prevSum1) */
            idx_t tmp = i;
            int prevRow  = tmp / (SUMS * SUMS);
            tmp -= prevRow * (SUMS * SUMS);
            int prevSum0 = tmp / SUMS;
            int prevSum1 = tmp % SUMS;

            /* try each possible next row */
            for (j = 0; j < ROWS; ++j) {
                int diff0 = (int)row_digits[prevRow][1] - (int)row_digits[j][0];
                int diff1 = (int)row_digits[prevRow][2] - (int)row_digits[j][1];
                if (diff0 > diff1)
                    continue;

                int newSum0 = (int)row_digits[prevRow][0] + (int)row_digits[j][1];
                int newSum1 = (int)row_digits[prevRow][1] + (int)row_digits[j][2];
                if (newSum0 < prevSum0 || newSum1 < prevSum1)
                    continue;
                /* sums are guaranteed to be < SUMS, but check safety */
                if (newSum0 >= SUMS || newSum1 >= SUMS)
                    continue;

                idx_t newState = (j * SUMS + newSum0) * SUMS + newSum1;
                dp_next[newState] += cnt;
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        for (i = 0; i < STATES; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all counts in the final state table */
    seq_t result = 0;
    for (i = 0; i < STATES; ++i)
        result += dp_cur[i];
    return result;
}
