/*
 * A243640  Number of length n+2 0..9 arrays with no three unequal elements in
 *          a row and new values 0..9 introduced in 0..9 order.
 * https://oeis.org/A243640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A243640: number of length n+2 arrays over digits 0..9
 * with (1) the first occurrence of each new digit appearing in order
 * 0,1,2,...,9 (restricted growth) and (2) no three consecutive
 * elements all distinct.  
 * The recurrence uses dynamic programming over states (a,b,m)
 * where a,b are the last two elements and m is the current maximum.
 * a(1) corresponds to length 3, a(2) to length 4, etc.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n = 0 */

    /* Required array length = n + 2 */
    size_t L_needed = (size_t)n + 2;

    /* DP arrays: dp_prev holds states for current length,
       dp_curr will hold states for next length. */
    static seq_t dp_prev[10][10][10];
    static seq_t dp_curr[10][10][10];

    static size_t cur_L = 0;          /* length of arrays already processed */
    static seq_t *vals = NULL;        /* cached a(n) values, 1‑based */
    static size_t vals_cap = 0;       /* allocated capacity of vals */

    /* Initialise at first call */
    if (cur_L == 0) {
        memset(dp_prev, 0, sizeof(dp_prev));
        /* After the first element (which must be 0),
           the last two elements are both 0 and the maximum is 0. */
        dp_prev[0][0][0] = 1;
        cur_L = 1;
    }

    /* Extend DP up to the required length */
    while (cur_L < L_needed) {
        /* Clear next state */
        memset(dp_curr, 0, sizeof(dp_curr));

        /* Transition from dp_prev to dp_curr */
        for (int a = 0; a < 10; ++a) {
            for (int b = 0; b < 10; ++b) {
                for (int m = 0; m < 10; ++m) {
                    seq_t cnt = dp_prev[a][b][m];
                    if (!cnt) continue;

                    int max_allowed = (m + 1 <= 9) ? (m + 1) : 9;
                    for (int z = 0; z <= max_allowed; ++z) {
                        /* Skip forbidden triple a,b,z all distinct */
                        if (a != b && b != z && a != z) continue;

                        int new_m = (m > z) ? m : z;
                        dp_curr[b][z][new_m] += cnt;
                    }
                }
            }
        }

        /* Prepare for next iteration */
        memcpy(dp_prev, dp_curr, sizeof(dp_prev));
        ++cur_L;

        /* Store a(n) when cur_L corresponds to n = cur_L - 2 */
        if (cur_L >= 3) {
            size_t idx = cur_L - 2;              /* n value */
            seq_t sum = 0;
            for (int a = 0; a < 10; ++a)
                for (int b = 0; b < 10; ++b)
                    for (int m = 0; m < 10; ++m)
                        sum += dp_prev[a][b][m];

            if (idx > vals_cap) {
                size_t new_cap = vals_cap ? (vals_cap << 1) : 16;
                while (new_cap < idx)
                    new_cap <<= 1;
                seq_t *new_arr = (seq_t *)realloc(vals,
                                                  (new_cap + 1) * sizeof(seq_t));
                if (!new_arr) return 0; /* allocation failure */
                if (!vals)
                    memset(new_arr, 0, (new_cap + 1) * sizeof(seq_t));
                else
                    memset(new_arr + vals_cap + 1,
                           0, (new_cap - vals_cap) * sizeof(seq_t));
                vals = new_arr;
                vals_cap = new_cap;
            }
            vals[idx] = sum;
        }
    }

    /* Return cached value */
    return vals[(size_t)n];
}
