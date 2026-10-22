/*
 * A301903  Number of nX5 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301903
   Recurrence: a(n) = 4*a(n-1) + 5*a(n-2) - 20*a(n-3) - 4*a(n-4) + 16*a(n-5)  for n > 6
   Initial values: a(1)=1, a(2)=3, a(3)=2, a(4)=8, a(5)=12, a(6)=64 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;      /* cache of computed terms */
    static idx_t  dp_len = 0;       /* highest index currently stored */

    /* n = 0 is outside the sequence (offset is 1) */
    if (n == 0)
        return 0;

    /* first call: allocate storage for indices 0..6 and set initial values */
    if (dp == NULL) {
        dp = (seq_t *)malloc((size_t)7 * sizeof(seq_t));   /* indices 0..6 */
        if (dp == NULL)
            return 0;               /* allocation failure */

        dp[0] = 0;                   /* unused, keep as zero */
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)3;
        dp[3] = (seq_t)2;
        dp[4] = (seq_t)8;
        dp[5] = (seq_t)12;
        dp[6] = (seq_t)64;
        dp_len = 6;
    }

    /* if requested term is already known, return it */
    if (n <= dp_len)
        return dp[n];

    /* extend the array to include index n */
    size_t new_len = (size_t)(n + 1);                   /* elements 0..n */
    if (new_len > (size_t)(dp_len + 1)) {
        seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
        if (tmp == NULL) {                            /* realloc failed */
            free(dp);
            dp    = NULL;
            dp_len = 0;
            return 0;
        }
        dp = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = dp_len + 1; i <= n; ++i) {
        seq_t val = dp[i-1] * (seq_t)4
                  + dp[i-2] * (seq_t)5
                  - dp[i-3] * (seq_t)20
                  - dp[i-4] * (seq_t)4
                  + dp[i-5] * (seq_t)16;
        dp[i] = val;
    }

    dp_len = n;               /* highest index now computed */

    return dp[n];
}
