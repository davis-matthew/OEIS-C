/*
 * A356542  Expansion of e.g.f. Product_{k>0} 1/(1 - k! * x^k)^(1/k!).
 * https://oeis.org/A356542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356542: Expansion of the e.g.f. Product_{k>0} 1/(1 - k! * x^k)^(1/k!).
   Recurrence (OEIS definition):
   a(0) = 1
   a(n) = (n-1)! * Σ_{k=1..n} A356541(k) * a(n-k)/(n-k)!   (n≥1)
   where A356541(k) = Σ_{d|k} d * (d!)^(k/d-1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized storage for a(n) */
    static seq_t *A = NULL;        /* a[0..A_cap-1] */
    static size_t A_cap = 0;       /* allocated slots */
    static idx_t  A_max = 0;       /* highest index with computed value */

    /* Ensure storage for a[n] exists */
    if (n + 1 > A_cap) {
        A = (seq_t*)realloc(A, (n + 1) * sizeof(seq_t));
        if (A == NULL) return 0;      /* allocation error */
        for (size_t i = A_cap; i <= n; ++i) A[i] = (seq_t)0;
        A_cap = n + 1;
    }

    /* Base case */
    if (n == 0) {
        if (A_cap == 0) {
            A = (seq_t*)malloc(sizeof(seq_t));
            if (A == NULL) return 0;
            A[0] = (seq_t)1;
            A_cap = 1;
            A_max = 0;
        } else if (A[0] == (seq_t)0) {
            A[0] = (seq_t)1;
        }
        A_max = 0;
        return (seq_t)1;
    }

    /* Return cached value if already computed */
    if (n <= A_max) {
        return A[n];
    }

    /* Compute factorials up to n */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!fact) return 0;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Compute B[k] = A356541(k) for 1 <= k <= n */
    seq_t *B = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!B) { free(fact); return 0; }
    for (idx_t d = 1; d <= n; ++d) {
        seq_t pow = (seq_t)1;
        for (idx_t k = d; k <= n; k += d) {
            B[k] += (seq_t)d * pow;
            pow *= fact[d];
        }
    }

    /* Compute a[i] for i = A_max+1 .. n */
    for (idx_t i = A_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t p   = (seq_t)1;            /* product (i-1)!/(j)! during loop */
        for (idx_t j = i - 1; ; --j) {
            idx_t k = i - j;
            seq_t term = B[k] * A[j];
            term *= p;
            sum += term;
            if (j == 0) break;
            p *= (seq_t)j;
        }
        A[i] = sum;
    }
    A_max = n;

    /* Clean up temporary arrays */
    free(fact);
    free(B);

    return A[n];
}
