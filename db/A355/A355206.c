/*
 * A355206  E.g.f. A(x) satisfies A'(x) = 1 + 2 * A(exp(x) - 1).
 * https://oeis.org/A355206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = 2 * Sum_{k=1..n} Stirling2(n,k) * a(k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, exit */
#include <stddef.h>     /* size_t */

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *A = NULL;     /* array of a(n) values  (1‑based) */
    static seq_t *S = NULL;     /* current Stirling‑2 row S(n,k) */
    static size_t A_cap = 0;    /* maximum index currently stored for A */
    static size_t S_cap = 0;    /* array size for S */
    static idx_t comp = 0;      /* largest n for which A[comp] is computed */

    /* Initialisation on first call */
    if (A == NULL) {
        A_cap = 1;             /* need index 1 at least */
        A = (seq_t*)malloc(sizeof(seq_t) * (A_cap + 1));
        if (A == NULL) exit(1);
        S_cap = 1;
        S = (seq_t*)malloc(sizeof(seq_t) * (S_cap + 1));
        if (S == NULL) { free(A); exit(1); }

        /* base values */
        A[1] = (seq_t)1;        /* a(1) = 1 */
        S[0] = (seq_t)0;        /* unused but keep zero */
        S[1] = (seq_t)1;        /* S(1,1) = 1 */
        comp = 1;
    }

    if (n < 1) return (seq_t)0;    /* behaviour below offset undefined */

    /* Grow the cache if necessary */
    if (n > A_cap) {
        size_t old_cap = A_cap;
        size_t new_cap = old_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

        seq_t *tmpA = (seq_t*)realloc(A, sizeof(seq_t) * (new_cap + 1));
        if (tmpA == NULL) exit(1);
        A = tmpA; A_cap = new_cap;

        seq_t *tmpS = (seq_t*)realloc(S, sizeof(seq_t) * (new_cap + 1));
        if (tmpS == NULL) exit(1);
        S = tmpS; S_cap = new_cap;

        /* initialise new S slots to zero (existing values may be garbage) */
        for (idx_t i = (idx_t)old_cap + 1; i <= (idx_t)new_cap; ++i) {
            S[i] = (seq_t)0;
        }
        S[0] = (seq_t)0;     /* keep the zero entry */
    }

    /* Compute terms up to n, if not yet cached */
    while (comp < n) {
        idx_t next = comp + 1;   /* we need a(next) */

        /* Ensure capacity for the new index (already done above) */

        /* Compute the sum Σ_{k=1}^{comp} S(comp,k) * a(k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= comp; ++k) {
            sum += S[k] * A[k];
        }
        A[next] = sum * ((seq_t)2);   /* a(next) = 2 * sum */

        /* Update S array to the next row: S(next, k) */
        /* Ensure S[next] starts at 0 for the recurrence */
        S[next] = (seq_t)0;
        for (idx_t k = next; k >= 1; --k) {
            /* descending order guarantees use of old values */
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }

        comp = next;      /* now a[comp] is known */
    }

    return A[n];
}
