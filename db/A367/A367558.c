/*
 * A367558  Number of knight paths from opposite corners of an n X n matrix,
 *          using only turns that lower Manhattan distance.
 * https://oeis.org/A367558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A367558.
   a(n) = number of knight paths from (0,0) to (n-1,n-1) on an n×n board,
   using only moves that reduce Manhattan distance (recurrence from F). */

/* Standard library for memory allocation */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 in the OEIS entry */
    if (n == 0)
        return 0;

    /* Board size is n × n, coordinates range 0 … n-1 */
    const idx_t N = n;                       /* board dimension */
    const idx_t maxSum = 2 * (n - 1);        /* largest antidiagonal s */

    /* Allocate 4 antidiagonals, each of length N */
    seq_t *dp = (seq_t *)malloc((size_t)4 * N * sizeof(seq_t));
    if (!dp)
        return 0;                            /* allocation failure unlikely */

    /* Zero‑initialize the array (RVO on most systems, but explicit for safety) */
    for (size_t i = 0; i < (size_t)4 * N; ++i)
        dp[i] = 0;

    /* Process antidiagonals in increasing order of s = a + b */
    for (idx_t s = 0; s <= maxSum; ++s) {
        const size_t cur  = s & 3;          /* current antidiagonal index (0‑3)   */
        const size_t pre1 = (s + 3) & 3;    /* s-1 (mod 4)   */
        const size_t pre3 = (s + 1) & 3;    /* s-3 (mod 4)   */

        /* Pointers to the relevant antidiagonals */
        seq_t *dp_cur   = dp + cur  * N;
        seq_t *dp_pre1  = dp + pre1 * N;
        seq_t *dp_pre3  = dp + pre3 * N;

        /* Valid values of a for this particular s */
        idx_t a_min = (s > (N - 1)) ? (s - (N - 1)) : 0;
        idx_t a_max = (s < (N - 1)) ? s : (N - 1);

        for (idx_t a = a_min; a <= a_max; ++a) {
            idx_t b = s - a;                /* b is automatically in [0,N-1] */

            if (s == 0) {                  /* base case (0,0) */
                dp_cur[a] = 1;
                continue;
            }

            seq_t sum = 0;

            /* (a-1,b-2)   from antidiagonal s-3 */
            if (a >= 1 && b >= 2)
                sum += dp_pre3[a - 1];

            /* (a-2,b-1)   from antidiagonal s-3 */
            if (a >= 2 && b >= 1)
                sum += dp_pre3[a - 2];

            /* (a+1,b-2)   from antidiagonal s-1 */
            if (a + 1 < N && b >= 2)
                sum += dp_pre1[a + 1];

            /* (a-2,b+1)   from antidiagonal s-1 */
            if (a >= 2 && b + 1 < N)
                sum += dp_pre1[a - 2];

            dp_cur[a] = sum;
        }
    }

    /* The desired value is located at a = n-1 on the last antidiagonal */
    seq_t result = dp[(maxSum & 3) * N + (N - 1)];
    free(dp);
    return result;
}
