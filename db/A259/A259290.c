/*
 * A259290  Number of (n+1) X (1+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0001 0101 or 0111.
 * https://oeis.org/A259290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-2),  a(1) = 10,  a(2) = 26 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp      = NULL;   /* dynamic cache of computed values   */
    static idx_t  dp_size = 0;     /* allocated array size (>= n+2)      */
    static idx_t  dp_max  = 0;     /* highest index that has been computed */

    if (n == 0)                     /* sequence offset starts at 1        */
        return (seq_t)0;

    /* Ensure the cache is large enough for indices up to n (plus one extra) */
    if (dp == NULL) {
        dp_size = n + 2;
        dp = (seq_t *)malloc(sizeof(seq_t) * dp_size);
    } else if (dp_size <= n + 1) {
        idx_t new_size = dp_size * 2;
        if (new_size <= n + 1)
            new_size = n + 2;
        dp = (seq_t *)realloc(dp, sizeof(seq_t) * new_size);
        dp_size = new_size;
    }

    /* Initialise the first two terms if they have not yet been set */
    if (dp_max < 2) {
        dp[1] = (seq_t)10;
        dp[2] = (seq_t)26;
        dp_max = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = dp_max + 1; i <= n; ++i) {
        seq_t tmp = dp[i - 2];
        tmp = tmp * (seq_t)4;          /* 4*a(i-2)   */
        dp[i] = dp[i - 1] + tmp;       /* a(i-1)+4*a(i-2) */
    }
    dp_max = n;

    return dp[n];
}
