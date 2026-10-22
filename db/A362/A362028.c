/*
 * A362028  a(n) = Sum_{k=1..n} (-1)^(n-k) * mu(k)^2, where mu(k) is the
 *          Moebius function.
 * https://oeis.org/A362028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * mu(k)^2, equivalently a(1)=1,
 *            a(n) = -a(n-1) + is_squarefree(n) for n>1
 * Range:     i8 n <= 632, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t computed = 0;
    static idx_t cap     = 0;

    /* Allocate or grow the cache array as needed. */
    if (n > cap) {
        idx_t new_cap = cap == 0 ? 1 : cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        A = tmp;
        cap = new_cap;
    }

    /* Compute missing terms incrementally. */
    if (computed < n) {
        if (computed == 0) {                /* base value a(1) = 1 */
            A[1] = (seq_t)1;
            computed = 1;
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t mu = mobius(i);
            seq_t sq = (mu != 0) ? (seq_t)1 : (seq_t)0;   /* 1 if i is square‐free */
            A[i] = -A[i - 1] + sq;
        }
        computed = n;
    }

    return A[n];
}
