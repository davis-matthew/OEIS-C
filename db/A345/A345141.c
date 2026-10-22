/*
 * A345141  a(1) = 1, a(2) = 0; a(n+2) = Sum_{d|n} a(d).
 * https://oeis.org/A345141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=0; a(n+2) = Σ_{d|n} a(d)  (n≥1) */
#include <stdlib.h>

/* Computes a(n) for n ≥ 1.  Results are memoised; calls may be in any order. */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1 */
        return 0;

    /* Static memoisation storage */
    static seq_t *A = NULL;   /* A[1..] holds a(k) */
    static idx_t last = 0;    /* highest n already computed */

    /* Ensure capacity for index n */
    if (A == NULL || (size_t)n + 1 > (size_t)last + 1) {
        size_t old_cap = (last ? (size_t)last + 1 : 0);
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 8)               /* favour some minimum size */
            new_cap = 8;
        if (new_cap < old_cap * 2)
            new_cap = old_cap * 2;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – return 0 (though callers may crash) */
            return 0;
        A = tmp;

        /* Initialise new slots to 0 */
        for (size_t i = old_cap; i < new_cap; ++i)
            A[i] = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k == 1) {
            A[1] = 1;
        } else if (k == 2) {
            A[2] = 0;
        } else {
            /* a(k) = sum_{d|(k-2)} a(d) */
            idx_t target = k - 2;
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= target; ++d) {
                if (target % d == 0) {
                    sum += A[d];
                    idx_t d2 = target / d;
                    if (d2 != d)
                        sum += A[d2];
                }
            }
            A[k] = sum;
        }
    }
    last = n;
    return A[n];
}
