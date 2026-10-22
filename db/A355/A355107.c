/*
 * A355107  E.g.f. A(x) satisfies: A(x) = 1 + 3 * x * A(-log(1-x)).
 * https://oeis.org/A355107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0, a(n) = 3 * n * sum_{k=0}^{n-1}
 *            |Stirling1(n-1,k)| * a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *A   = NULL;  /* array of a-values                */
    static seq_t *S   = NULL;  /* array of unsigned Stirling1 row  */
    static idx_t  cap = 0;     /* allocated capacity for A and S   */
    static idx_t  last = 0;    /* largest n already computed       */
    static seq_t  T   = 0;     /* T = sum_{k=0}^{last} s(last,k)*a(k) */

    /* Allocate or grow arrays if necessary */
    if (cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));

        /* initialise new entries */
        for (idx_t i = cap; i < new_cap; ++i) {
            A[i] = 0;
            S[i] = 0;
        }
        cap = new_cap;

        /* initialise base case if we have never computed anything yet */
        if (last == 0 && cap >= 1) {
            A[0] = 1;
            S[0] = 1;      /* s(0,0) = 1 */
            T   = 1;       /* T_0 = s(0,0)*a(0) = 1 */
        }
    }

    /* Compute values up to requested n (if not already done) */
    for (idx_t cur = last + 1; cur <= n; ++cur) {
        /* a(cur) = 3*cur*T_{cur-1} */
        seq_t ak = 3 * (seq_t)cur * T;
        A[cur] = ak;

        /* Compute U = sum_{k=0}^{cur-1} s(cur-1,k) * a(k+1) */
        seq_t U = 0;
        for (idx_t k = 0; k < cur; ++k) {
            U += S[k] * A[k + 1];
        }

        /* T_cur = U + (cur-1)*T_{cur-1} */
        seq_t T_next = U + (cur - 1) * T;

        /* Update Stirling row from n-1 to n in-place (high to low) */
        for (idx_t k = cur; (idx_t)k > 0; --k) {
            S[k] = S[k - 1] + (cur - 1) * S[k];
        }
        S[0] = 0;   /* s(cur,0) = 0 for cur > 0 */

        /* Prepare for next iteration */
        T = T_next;
    }

    last = n;
    return A[n];
}
