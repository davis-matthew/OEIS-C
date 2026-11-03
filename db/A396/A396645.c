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
 * Validated: standard, pass, 34 terms through n = 57 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
{
    /* static memoisation tables */
    static seq_t *A = NULL;          /* a values, A[0] = a_wide(0) */
    static size_t A_cap = 0;
    static size_t A_len = 0;         /* number of stored a values */

    static seq_t *S = NULL;          /* current Stirling row S(n-1,k) */
    static size_t S_cap = 0;         /* capacity of S array */

    /* initialise for n=0 */
    if (!A) {
        A_cap = 1;
        A_len = 1;
        A = mem_new(seq_t, A_cap);
        if (!A) return 0;           /* out of memory */
        A[0] = 1;                    /* a_wide(0) = 1 */

        S_cap = 1;
        S = mem_new(seq_t, S_cap);
        if (!S) { mem_free(A); return 0; }
        S[0] = 1;                    /* S(0,0) = 1 */
    }

    /* compute all a values up to requested n */
    while (A_len <= n) {
        idx_t cur = A_len;               /* compute a[cur]   */

        /* ensure S has capacity for cur+1 elements (k = 0..cur) */
        if (S_cap <= cur) {
            size_t new_cap = S_cap ? S_cap : 1;
            while (new_cap <= cur) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, S, new_cap);
            if (!tmp) { mem_free(A); mem_free(S); return 0; }
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
            seq_t *tmp = mem_resize(seq_t, A, new_cap);
            if (!tmp) { mem_free(A); mem_free(S); return 0; }
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
