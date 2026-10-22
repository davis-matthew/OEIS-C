/*
 * A394842  G.f. A(x) satisfies A(x) = 1 / (1 - x * d/dx log(1 + x*A(x)^2)).
 * https://oeis.org/A394842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=0..n-1} a(k)*a(n-1-k) + (2*(n-1)/3) *
 *            Sum_{i+j+k=n-1} a(i)*a(j)*a(k) (n≥1) Offset: 0 (a(0) is the
 *            first term listed)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/seq_alloc_new_array.h"   /* allocates zero‑initialised seq_t arrays */

/* Static cache of values and pair convolution table */
static idx_t last = (idx_t)-1;           /* highest n computed */
static seq_t *A = NULL;                 /* a[n]    */
static seq_t *C2 = NULL;                /* C2[t] = Σ_{i+j=t} a[i]*a[j] */
static size_t capA = 0;                 /* capacity of A   */
static size_t capC = 0;                 /* capacity of C2  */

/* Ensure the internal arrays are large enough for indices up to n and 2*n */
static void ensure_capacity(idx_t n) {
    if (capA <= n) {
        size_t newcap = capA ? capA * 2 : 1;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *newA = seq_alloc_new_array(newcap);
        for (size_t i = 0; i < capA; ++i) newA[i] = A[i];
        if (A) free(A);
        A = newA;
        capA = newcap;
    }
    if (capC <= (size_t)(2 * n)) {
        size_t newcap = capC ? capC * 2 : 1;
        if (newcap <= (size_t)(2 * n)) newcap = (size_t)(2 * n) + 1;
        seq_t *newC = seq_alloc_new_array(newcap);
        for (size_t i = 0; i < capC; ++i) newC[i] = C2[i];
        if (C2) free(C2);
        C2 = newC;
        capC = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* One‑time initialization */
    if (last == (idx_t)-1) {
        /* allocate minimal space for a[0] and C2[0] */
        ensure_capacity(0);
        A[0]   = (seq_t)1;
        C2[0]  = (seq_t)1;   /* a0*a0 */
        last   = 0;
    }

    if (n <= last) return A[n];

    /* Make sure we can store up to n and 2*n */
    ensure_capacity(n);

    /* Compute values from last+1 up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        idx_t prev = m - 1;

        /* Pair convolution sum for current index */
        seq_t sumPairs = C2[(size_t)prev];

        /* Triple convolution: Σ_{i=0..prev} a[i] * C2[prev-i] */
        seq_t tri = 0;
        for (idx_t i = 0; i <= prev; ++i)
            tri += A[(size_t)i] * C2[(size_t)(prev - i)];

        /* a(m) = sumPairs + (2*(m-1)/3) * tri */
        seq_t factor = (seq_t)(2 * prev);
        seq_t part   = (factor * tri) / 3;
        A[(size_t)m] = sumPairs + part;

        /* Update pair convolution table with the new term a[m] */
        for (idx_t k = 0; k <= m; ++k) {
            idx_t t = m + k;
            seq_t prod = A[(size_t)m] * A[(size_t)k];
            if (k == m) {
                C2[(size_t)t] += prod;
            } else {
                C2[(size_t)t] += prod + prod;  /* two ordered pairs */
            }
        }
    }

    last = n;
    return A[(size_t)n];
}
