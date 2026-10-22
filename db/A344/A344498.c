/*
 * A344498  a(n) = |Stirling1(n, floor(n/2))| * floor(n/2)!.
 * https://oeis.org/A344498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)! * |Stirling1(n, floor(n/2))|
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;

    /* Allocate an array of k+1 unsigned Stirling numbers of the first kind
       processed row by row.  We keep only the values needed for the
       current row. */
    seq_t *s = (seq_t *)malloc((size_t)(k + 1) * sizeof(seq_t));
    if (!s) abort();                      /* allocation failure protection */

    /* initialize s[0] = 1 (for n = 0), all other entries 0. */
    s[0] = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) s[i] = (seq_t)0;

    /* Perform the recurrence
          s(i,j) = s(i-1,j-1) + (i-1)*s(i-1,j)
       iterating over i from 1 to n.  The array s[j] always
       holds the values of the previous row (i-1, j).  The update
       is done in reverse j order so that s[j-1] refers to the
       previous row's value, not the one updated in this iteration. */
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t maxj = (i < k) ? i : k;
        for (idx_t j = maxj; j >= 1; --j)
        {
            seq_t old = s[j];                       /* s(i-1,j)   */
            s[j] = s[j - 1] + (seq_t)(i - 1) * old; /* s(i,j)     */
        }
        s[0] = (seq_t)0;   /* s(i,0) = 0 for i > 0 */
    }

    /* Compute k! (factorial of floor(n/2)). */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        fact *= (seq_t)i;

    seq_t result = s[k] * fact;     /* |Stirling1(n,k)| * k! */

    free(s);
    return result;
}
