/*
 * A377426  E.g.f. satisfies A(x) = 1/(1 + log(1 - x*A(x)^4)).
 * https://oeis.org/A377426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377426
   Formula: a(n) = (1/(4*n+1)!) * Sum_{k=0..n} (4*n+k)! * |Stirling1(n,k)|
   Recurrence: produce unsigned Stirling numbers of the first kind and use
   the above closed form linearly.  The implementation memoises both the
   Stirling row and the sequence values to handle arbitrary call order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised data */
    static seq_t *A = NULL;          /* a values */
    static seq_t *T = NULL;          /* current Stirling row s(n,k) */
    static idx_t  max_n = (idx_t)-1;
    static size_t cap   = 0;         /* capacity of T and A arrays */

    /* Initialise for n = 0 */
    if (max_n == (idx_t)-1) {
        max_n = 0;
        cap   = 1;
        A = (seq_t*)malloc(sizeof(seq_t) * cap);
        T = (seq_t*)malloc(sizeof(seq_t) * cap);
        A[0] = 1;   /* a(0) = 1 */
        T[0] = 1;   /* s(0,0) = 1 */
    }

    /* Ensure sufficient capacity */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        T = (seq_t*)realloc(T, sizeof(seq_t) * new_cap);
        A = (seq_t*)realloc(A, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* If the requested value is already known, return it immediately */
    if (n <= max_n) return A[n];

    /* Expand memoisation up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* --- Update the Stirling row for n = m --- */
        /* T currently holds s(m-1,k) for k=0..m-1.  We will overwrite in place. */
        T[m] = 0;                     /* new element for s(m,m) */
        seq_t factor = (seq_t)(m - 1);
        for (idx_t k = m; k >= 1; --k) {
            seq_t left  = T[k - 1];
            seq_t right = T[k];
            T[k] = left + factor * right;
        }
        T[0] = 0;                     /* s(m,0) = 0 for m > 0 */

        /* --- Compute a(m) using the closed form --- */
        if (m == 0) {                 /* already set during init */
            A[0] = 1;
            continue;
        }

        seq_t prod = 1;                /* product_{t=2..k} (4*m + t) */
        seq_t sum  = 0;
        for (idx_t k = 1; k <= m; ++k) {
            if (k >= 2) {
                uint64_t mult = 4ULL * m + k; /* small multiplier fits 64‑bit */
                prod = prod * (seq_t)mult;
            }
            sum = sum + prod * T[k];
        }
        A[m] = sum;
    }

    max_n = n;
    return A[n];
}
