/*
 * A376056  Lexicographically earliest sequence of positive integers a(1),
 *          a(2), a(3), ... such that for any n > 0, S(n) = Sum_{k = 1..n}
 *          (2*k-1)/a(k) < 1.
 * https://oeis.org/A376056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2*n-1)/ ... Recurrence used: a(1) = 2 e(1) = 2 For n ≥
 *            1: a(n+1) = (2*n+1)*e(n) + 1 e(n+1) = (2*n+1)*e(n)*e(n) + e(n)
 *            Thus a(n) can be generated iteratively, storing both a and e.
 *            The function memoises the sequence so any call order is
 *            supported
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation arrays, zero indexed but index 0 unused */
    static seq_t *A = NULL;
    static seq_t *E = NULL;
    static idx_t  cap = 0;   /* number of allocated entries (≥1 if non-NULL) */

    if (n == 0)
        return 0;   /* not defined in OEIS, but avoid UB */

    if (n >= cap) {
        /* enlarge allocations to include index n */
        size_t new_cap = (size_t)(n + 1);   /* +1 so that index n is valid */
        if (A == NULL) {
            A = (seq_t *)malloc(new_cap * sizeof(seq_t));
            E = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!A || !E) exit(EXIT_FAILURE);
        } else {
            seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            seq_t *tmpE = (seq_t *)realloc(E, new_cap * sizeof(seq_t));
            if (!tmpA || !tmpE) exit(EXIT_FAILURE);
            A = tmpA;  E = tmpE;
        }

        /* compute new entries */
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 1) {
                A[1] = 2;
                E[1] = 2;
            } else {
                idx_t j = i - 1;
                seq_t factor = (seq_t)(2 * j + 1);
                A[i] = factor * E[j] + 1;
                E[i] = factor * E[j] * E[j] + E[j];
            }
        }
        cap = new_cap;
    }
    return A[n];
}
