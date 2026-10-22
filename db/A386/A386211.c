/*
 * A386211  G.f. A(x) satisfies A(x) = 1/(1-x)^2 + x^2*A(x)*A'(x).
 * https://oeis.org/A386211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = n + 1 + ((n-1) * sum_{k=0}^{n-1} a(k)*a(n-1-k))
 *            / 2 (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A386211.  The function is memoised so that each term
   is calculated only once, regardless of call order. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoised array */
    static seq_t *A = NULL;     /* holds a(i) for i = 0 .. cur */
    static size_t cap = 0;      /* allocated capacity */
    static idx_t cur = 0;       /* max index already computed */

    /* initialise array on first call */
    if (A == NULL) {
        cap = 1;
        A = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!A) return (seq_t)0;    /* out‑of‑memory: return 0 */
        A[0] = (seq_t)1;            /* a(0) = 1 */
        cur = 0;
    }

    /* grow storage if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out‑of‑memory */
        A = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cur + 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
            sum += A[k] * A[i - 1 - k];

        seq_t prod = ((seq_t)(i - 1)) * sum;   /* (n‑1) * sum */
        seq_t add  = prod / (seq_t)2;          /* integer division by 2 */

        A[i] = (seq_t)i + (seq_t)1 + add;
    }

    cur = n;
    return A[n];
}
