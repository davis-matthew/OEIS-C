/*
 * A383895  Number of spiny partitions with exactly n parts.
 * https://oeis.org/A383895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of spiny partitions with exactly n parts
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP below counts solutions of the system                                  */
/*   sum_{k≥1} m_k = n                                                        */
/*   for k≥2: 0 ≤ m_k ≤ floor( S(k-1)/(k-1) )                                 */
/* where S(k-1)=m_1+…+m_{k-1}. This follows from the definition of spiny  */
/* partitions and gives a correct and efficient O(n^2) algorithm for n≤10000.  */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t N = n;
    /* allocate two work arrays of length N+1 */
    seq_t *dp   = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp || !next) return (seq_t)0; /* allocation failed */

    dp[0] = (seq_t)1;    /* start with zero parts */

    for (idx_t k = 1; k <= N; ++k)
    {
        /* first zero target array */
        memset(next, 0, (size_t)(N + 1) * sizeof(seq_t));

        if (k == 1)
        {
            /* k=1 has no upper bound on m_1 except the total number of parts.
               The update reduces to a cumulative sum of dp. */
            seq_t acc = (seq_t)0;
            for (idx_t cap = 0; cap <= N; ++cap)
            {
                acc += dp[cap];
                next[cap] = acc;
            }
        }
        else
        {
            for (idx_t cap = 0; cap <= N; ++cap)
            {
                seq_t val = dp[cap];
                if (val == (seq_t)0) continue;

                /* maximal multiplicity allowed for part size k */
                idx_t max_m = cap / (k - 1);
                if (max_m > N - cap) max_m = N - cap;

                for (idx_t m = 0; m <= max_m; ++m)
                    next[cap + m] += val;
            }
        }

        /* swap dp and next for next iteration */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[N];   /* number of partitions with exactly N parts */
    free(dp);
    free(next);
    return result;
}
