/*
 * A339169  Number of compositions (ordered partitions) of n into distinct
 *          parts, the least being 6.
 * https://oeis.org/A339169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1, k*(k+11)/2 <= n} k! * p_{<=k-1}( n -
 *            k*(k+11)/2 ) where p_{<=m}(t) is the number of partitions of t
 *            into parts not exceeding m. The implementation iteratively
 *            builds the partition counts for increasing maximum part sizes
 *            and accumulates the result
 * Range:     u8 n <= 37, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* At most n+1 elements needed for temporary DP array */
    size_t len = (size_t)(n + 1);
    seq_t *p = (seq_t*)malloc(len * sizeof(seq_t));
    if (!p) return 0;                      /* allocation failure */

    /* initialise partition counts for parts up to 0 (empty set) */
    for (size_t i = 0; i < len; ++i) p[i] = 0;
    p[0] = 1;

    seq_t fact = 1;   /* k! for current k (starts with 1! when k==1) */
    seq_t ans  = 0;

    for (idx_t k = 1; ; ++k)
    {
        seq_t min_sum = (seq_t)k * (k + 11) / 2;
        if (min_sum > (seq_t)n) break;

        idx_t need = n - (idx_t)min_sum;        /* remaining sum after choosing minimal parts */
        ans = ans + fact * p[need];            /* add contribution for this k */

        /* prepare factorial for next k: (k+1)! */
        fact = fact * (k + 1);

        /* Update partition counts to include parts of size k for future iterations */
        for (idx_t s = k; s <= n; ++s)
            p[s] = p[s] + p[s - k];
    }

    free(p);
    return ans;
}
