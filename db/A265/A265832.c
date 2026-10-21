/*
 * A265832  Expansion of Product_{k>=1} 1/(1 - (5*k-2)*x^(5*k-2)).
 * https://oeis.org/A265832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 62, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of Product_{k>=1} 1/(1 - (5*k-2)*x^(5*k-2)) */
/* Sequence a(n) is computed by dynamic programming over the part sizes m = 5*k-2: */
/*   a[0] = 1, a[n] = sum_{m<=n} a[n-m] * m                          */
/*   Each part size m may appear any number of times; the factor m^count is */
/*   incorporated by the inner loop that iterates n ascending.           */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array a[0..n] initialized to 0 */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp)
        return 0;                /* allocation failure – unlikely */

    dp[0] = 1;                    /* base case: empty partition */

    /* Process all part sizes m = 5*k-2 that are <= n */
    for (idx_t k = 1; ; ++k) {
        idx_t m = 5*k - 2;
        if (m > n)
            break;
        seq_t sm = (seq_t)m;       /* cast to seq_t for multiplication */
        for (idx_t i = m; i <= n; ++i) {
            dp[i] += dp[i - m] * sm;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
