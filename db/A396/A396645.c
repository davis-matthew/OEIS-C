/*
 * A396645  a(n) = Sum_{j=1..n} |Stirling1(n-1,j-1)| * a(n-j) for n>=1,
 *          starting with a(0) = 1.
 * https://oeis.org/A396645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} |Stirling1(n-1,j-1)| * a(n-j), a(0)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code memoises a and the unsigned Stirling numbers of the first kind
 * for all necessary indices.  Recurrence for the Stirling numbers:
 *   S(0,0)=1,  S(n,0)=0 (n>0),
 *   S(n,k)=S(n-1,k-1)+(n-1)*S(n-1,k)   (absolute values).
 * The algorithm maintains the array S holding the current row S(n-1,·)
 * and updates it in place to the next row after each a[n] is computed. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *A = NULL;          /* a values, A[0] = a(0) */
    static size_t A_cap = 0;
    static size_t A_len = 0;         /* number of stored a values */

    static seq_t *S = NULL;          /* current Stirling row S(n-1,k) */
    static size_t S_cap = 0;         /* capacity of S array */

    /* initialise for n=0 */
    if (!A) {
        A_cap = 1;
        A_len = 1;
        A = (seq_t *)malloc(sizeof(seq_t) * A_cap);
        if (!A) return 0;           /* out of memory */
        A[0] = 1;                    /* a(0) = 1 */

        S_cap = 1;
        S = (seq_t *)malloc(sizeof(seq_t) * S_cap);
        if (!S) { free(A); return 0; }
        S[0] = 1;                    /* S(0,0) = 1 */
    }

    /* compute all a values up to requested n */
    while (A_len <= n) {
        idx_t cur = A_len;               /* compute a[cur]   */

        /* ensure S has capacity for cur+1 elements (k = 0..cur) */
        if (S_cap <= cur) {
            size_t new_cap = S_cap ? S_cap : 1;
            while (new_cap <= cur) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(S, sizeof(seq_t) * new_cap);
            if (!tmp) { free(A); free(S); return 0; }
            S = tmp;
            for (idx_t i = S_cap; i <= cur; ++i) S[i] = 0;
            S_cap = new_cap;
        }

        /* compute a[cur] = sum_{k=0}^{cur-1} S(k) * a[cur-1-k] */
        seq_t val = 0;
        for (idx_t k = 0; k < cur; ++k) {
            val += S[k] * A[cur - 1 - k];
        }

        /* store a[cur] */
        if (A_len >= A_cap) {
            size_t new_cap = A_cap ? A_cap << 1 : 1;
            while (A_len >= new_cap) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(A, sizeof(seq_t) * new_cap);
            if (!tmp) { free(A); free(S); return 0; }
            A = tmp;
            A_cap = new_cap;
        }
        A[cur] = val;
        ++A_len;

        /* update S to represent S(cur,k) for next iteration:
           use multiplier cur-1 (since we need S(cur,k)) */
        for (idx_t k = cur; k > 0; --k) {
            seq_t left  = S[k - 1];
            seq_t mult  = (seq_t)(cur - 1) * S[k];
            S[k] = left + mult;
        }
        S[0] = 0;          /* S(cur,0) = 0 for cur > 0 */
    }

    return A[n];
}
