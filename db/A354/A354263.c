/*
 * A354263  Expansion of e.g.f. 1/(1 + 3 * log(1-x)).
 * https://oeis.org/A354263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A354263:  a(0)=1; a(n)=3*∑_{k=1..n} (k-1)!⋅C(n,k)⋅a(n-k) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *A   = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static idx_t   done  = 0;   /* number of elements already computed */

    /* ensure enough space */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        A = tmp;
        cap = new_cap;
    }

    /* initialise first term */
    if (done == 0) {
        A[0] = (seq_t)1;
        done = 1;
    }

    /* compute missing terms */
    for (idx_t i = done; i <= n; ++i)
    {
        seq_t sum  = 0;
        seq_t comb = 1;   /* binomial coefficient C(i, 0) */
        seq_t fact = 1;   /* (k-1)! for current k */

        for (idx_t k = 1; k <= i; ++k)
        {
            comb = comb * (seq_t)(i - k + 1) / (seq_t)k;  /* C(i,k) */
            seq_t term = fact;                           /* (k-1)!   */
            term  *= comb;                               /* *C(i,k)  */
            term  *= A[i - k];                           /* *a(i-k)  */
            sum   += term;                               /* accumulate   */
            fact  *= (seq_t)k;                           /* == (k)!   */
        }
        A[i] = sum * (seq_t)3;
    }

    done = n + 1;
    return A[n];
}
