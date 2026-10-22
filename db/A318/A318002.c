/*
 * A318002  E.g.f.: 2*cosh(x) / (1 + sqrt(1 - 2*sinh(2*x))).
 * https://oeis.org/A318002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A318002
 *  E.g.f.: A(x) = cosh(x) + sinh(x)A(x)^2
 *
 *  The sequence satisfies:
 *    a(0) = 1
 *    for n ≥ 1
 *      a(n) = [n even]  + ∑_{1≤t≤n, t odd}  binom(n,t) · d(n‑t)
 *
 *  where d(m) = ∑_{i=0}^m  binom(m,i) · a(i) · a(m‑i).
 *
 *  This recurrence is evaluated with memoisation; complexity is
 *  O(n²) which is adequate for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
extern seq_t a(idx_t n);  /* prototype to avoid implicit declaration */

/*  opaque helper for combinatorial coefficients   */
#include "../utils/binomial.h"

/*  Return a(n).  The function is memoised so that term
 *  computation is independent of call order.           */
seq_t
a(idx_t n)
{
    /* static state: memoised arrays and capacity */
    static seq_t *A   = NULL;   /* a[0..cap-1] */
    static seq_t *D   = NULL;   /* d[0..cap-1] */

    static size_t cap   = 0;    /* allocated size */
    static size_t last  = 0;    /* largest index computed (0‑based) */

    /* initialise capacity if needed */
    if (cap == 0) {
        cap = 1;
        A  = (seq_t *)malloc(cap * sizeof(seq_t));
        D  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!A || !D) return (seq_t)0;    /* memory error propagates */

        /* base cases */
        A[0] = (seq_t)1;
        D[0] = (seq_t)1;   /* a0*a0 */
        last = 0;
    }

    /* expand capacity to hold up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp;

        tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        A = tmp;

        tmp = (seq_t *)realloc(D, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        D = tmp;

        cap = new_cap;
    }

    /* compute terms up to n if not already done */
    for (idx_t k = last + 1; k <= n; ++k) {

        /* term = sum_{odd t <= k} binom(k,t) * d(k-t) */
        seq_t term = 0;
        for (idx_t t = 1; t <= k; t += 2) {            /* t odd only */
            seq_t bin = binomial(k, t);
            seq_t prod = bin * D[k - t];
            term += prod;
        }

        /* add contribution from cosh(x): 1 if k even, else 0 */
        if ((k & 1) == 0)
            term += (seq_t)1;

        A[k] = term;

        /* compute d(k) = sum_{i=0}^k binom(k,i) * a(i) * a(k-i) */
        seq_t dk = 0;
        for (idx_t i = 0; i <= k; ++i) {
            seq_t bin = binomial(k, i);
            seq_t prod = bin * A[i];
            prod *= A[k - i];
            dk += prod;
        }
        D[k] = dk;
    }

    last = (size_t)n;   /* last computed index for future calls */
    return A[n];
}
