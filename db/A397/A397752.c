/*
 * A397752  Number of unlabeled bridgeless polygonal snake graphs with n
 *          vertices.
 * https://oeis.org/A397752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 40, u64 n <= 74, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A397752 – Number of unlabeled bridgeless polygonal snake graphs with n vertices
   Recurrence (derived from the rational generating function):
   a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=2, a(6)=2, a(7)=4,
   a(8)=6, a(9)=11, a(10)=18, a(11)=34, a(12)=59, a(13)=112
   For n ≥ 14:
   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - a(n-4) - 2*a(n-5) - 3*a(n-6)
          + a(n-8) + a(n-9) + 2*a(n-10) - a(n-12)
*/

#include <stdlib.h>

static const seq_t _init[13] =
    {0, 0, 1, 1, 2, 2, 4, 6, 11, 18, 34, 59, 112};

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n <= 13) return _init[n - 1];

    /* The recurrence uses terms up to n‑12, so a simple dynamic array
       of size n+1 is enough for the expected n ≤ 10000. */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) abort();          /* allocation failure */

    /* initialise seeds 1..13 */
    for (size_t i = 1; i <= 13; ++i)
        dp[i] = _init[i - 1];

    /* recurrence for n ≥ 14 */
    for (size_t i = 14; i <= N; ++i)
        dp[i] = dp[i - 1]
                + 3 * dp[i - 2]
                - dp[i - 3]
                - dp[i - 4]
                - 2 * dp[i - 5]
                - 3 * dp[i - 6]
                + dp[i - 8]
                + dp[i - 9]
                + 2 * dp[i - 10]
                - dp[i - 12];

    seq_t result = dp[N];
    free(dp);
    return result;
}
