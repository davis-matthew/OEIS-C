/*
 * A389284  G.f. A(x) satisfies A(x) = 1 + x/(1-x^2)^2 * A(x)^3.
 * https://oeis.org/A389284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-3*k-1,k) *
 *            A001764(n-2*k) Recurrence derived from the generating function:
 *            a(n) - 2 a(n-2) + a(n-4) = b(n-1) + δ_{n0} - 2 δ_{n2} + δ_{n4}
 *            where b(m) = Σ_{i=0..m} a(i) * c(m-i) and c(k) = Σ_{i=0..k} a(i)
 *            * a(k-i) (coefficients of A(x)^2). This implementati...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static buffers and state */
    static seq_t *A = NULL;          /* coefficients a[0..max_n]      */
    static seq_t *C = NULL;          /* coefficients a[i]*a[j] for i+j=k (k ≤ max_n) */
    static size_t cap = 0;          /* current allocated capacity */
    static idx_t computed = 0;      /* number of terms already computed (≥1 if A[0] exists) */

    /* Allocate or grow buffers to hold at least n+1 terms */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap == 0 ? 1 : cap);
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;

        seq_t *new_A = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *new_C = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_A || !new_C) {
            /* Out of memory – return 0 (undefined for this sequence). */
            return (seq_t)0;
        }

        /* Copy existing data if any */
        if (A) memcpy(new_A, A, (size_t)computed * sizeof(seq_t));
        if (C) memcpy(new_C, C, (size_t)computed * sizeof(seq_t));

        /* Zero the freshly allocated parts */
        for (size_t i = computed; i < new_cap; ++i) {
            new_A[i] = (seq_t)0;
            new_C[i] = (seq_t)0;
        }

        /* Free old buffers and adopt new ones */
        if (A) free(A);
        if (C) free(C);
        A = new_A;
        C = new_C;
        cap = new_cap;
    }

    /* Initialize base case if this is the very first call */
    if (computed == 0) {
        A[0] = (seq_t)1;          /* a(0) = 1 */
        C[0] = (seq_t)1;          /* a(0)*a(0) = 1 */
        computed = 1;
    }

    /* Compute successive terms up to n (inclusive) */
    for (idx_t i = computed; i <= n; ++i) {
        /* --- compute b_{i-1} = Σ_{k=0..i-1} a(k) * C(i-1-k) --- */
        seq_t b = (seq_t)0;
        for (idx_t k = 0; k <= i - 1; ++k) {
            b += A[k] * C[i - 1 - k];
        }

        /* --- compute a(i) using the recurrence --- */
        seq_t ai = b;                     /* start with b_{i-1}  */
        if (i >= 2) ai += (seq_t)2 * A[i - 2];
        if (i >= 4) ai -= A[i - 4];
        if (i == 0) ai += (seq_t)1;      /* δ_{i0}   */
        if (i == 2) ai -= (seq_t)2;      /* -2 δ_{i2} */
        if (i == 4) ai += (seq_t)1;      /* + δ_{i4}  */
        A[i] = ai;

        /* --- compute C[i] = Σ_{k=0..i} a(k) * a(i-k) --- */
        seq_t ci = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            ci += A[k] * A[i - k];
        }
        C[i] = ci;
    }

    computed = n + 1;   /* update state */
    return A[n];
}
