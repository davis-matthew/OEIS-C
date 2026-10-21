/*
 * A277673  Number of n-length words over an n-ary alphabet {a_1,a_2,...,a_n}
 *          avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] 1/(1+Sum_{j=1..n} (n+1-j)*(-x)^j)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses a dynamic‑programming approach.
   dp[l][j] = number of words of length l that end with letter j.
   The recurrence is
        dp[1][j] = 1,
        dp[l+1][1] = total[l],
        dp[l+1][j] = total[l] - dp[l][j-1]   (j >= 2)
   where total[l] = Σ_{j=1..n} dp[l][j].
   The answer a(n) is total[n].

   The algorithm runs in O(n²) time and O(n) memory, which is
   sufficient for n up to about 10 000 even with arbitrary‑precision
   integers.  For small n it runs in microseconds, for n=10 000 it
   finishes within a few seconds on typical hardware. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the empty word case. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate two work arrays of size n+1.  The arrays are indexed
       from 1 to n; index 0 is unused.  The allocation method depends
       on whether we are compiling as C or C++.  For C++ big‑int
       types we must call constructors, so we use new[]; otherwise
       malloc/free are fine. */
#ifdef __cplusplus
    seq_t *dp_prev = new seq_t[n + 1];
    seq_t *dp_curr = new seq_t[n + 1];
#else
    seq_t *dp_prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp_prev || !dp_curr) return (seq_t)0;  /* allocation failed */
#endif

    /* Initial words of length 1: every letter is allowed. */
    for (idx_t j = (idx_t)1; j <= n; ++j) {
        dp_prev[j] = (seq_t)1;
    }

    /* total number of words of the current length.  For length 1 it is n. */
    seq_t total = (seq_t)n;

    /* Iterate over word lengths from 1 up to n‑1.  After the loop
       dp_prev will contain the counts for length n. */
    for (idx_t l = (idx_t)1; l < n; ++l) {
        /* Compute dp_curr based on dp_prev and current total. */
        dp_curr[(idx_t)1] = total;                /* dp[l+1][1] */
        for (idx_t j = (idx_t)2; j <= n; ++j) {
            dp_curr[j] = total - dp_prev[j - 1];   /* dp[l+1][j] */
        }

        /* Compute the new total as the sum of dp_curr. */
        seq_t new_total = (seq_t)0;
        for (idx_t j = (idx_t)1; j <= n; ++j) {
            new_total += dp_curr[j];
        }

        /* Prepare for the next iteration: swap the arrays. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
        total = new_total;
    }

    /* The answer is the total number of length‑n words. */
    seq_t result = total;

#ifdef __cplusplus
    delete[] dp_prev;
    delete[] dp_curr;
#else
    free(dp_prev);
    free(dp_curr);
#endif

    return result;
}
