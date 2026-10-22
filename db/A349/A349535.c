/*
 * A349535  G.f. A(x) satisfies A(x) = 1 / ((1 - 4 * x) * (1 - x * A(x)^2)).
 * https://oeis.org/A349535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4^n + Σ_{i+j+k=n-1} a(i)·a(j)·a(k)
 * (equivalently, a(n) = 4^n + Σ_{k=0..n-1} a(k)·(Σ_{i+j=n-1-k} a(i)·a(j))).
 * This uses the recurrence derived from the generating‑function
 *  A(x) = 1 / ((1-4x)(1-x·A(x)^2)).
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *A = NULL;        /* values a(0..max) */
    static seq_t *C2 = NULL;       /* C2[m] = Σ_{i+j=m} a(i)·a(j)   */
    static size_t cap = 0;         /* allocated capacity (>= max+1) */
    static idx_t computed = 0;     /* highest index computed so far */

    /* power of 4 (4^i) for the current i */
    static seq_t pow4 = 1;        /* 4^0 at start */

    /* --- allocation and growth -------------------------------------- */
    if (n + 1 > cap) {
        size_t newcap = (cap == 0) ? 64 : cap * 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        A  = (seq_t *)realloc(A,  newcap * sizeof(seq_t));
        C2 = (seq_t *)realloc(C2, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* --- initialise base values ------------------------------------- */
    if (computed == 0) {
        A[0]   = (seq_t)1;
        C2[0]  = (seq_t)1;        /* a0·a0 */
        pow4   = (seq_t)1;        /* 4^0 */
        computed = 0;
    }

    /* If we already have the answer, just return it. */
    if (n <= computed) return A[n];

    /* --- compute all missing values up to n ------------------------ */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* 4^i */
        pow4 = pow4 * (seq_t)4;

        /* Shared convolution part: C2[i-1] is already known (see below) */
        seq_t sum_triple = (seq_t)0;
        for (idx_t k = 0; k <= i - 1; ++k) {
            sum_triple += A[k] * C2[i - 1 - k];
        }

        /* a(i) = 4^i + Σ_{k} a(k)·C2[i-1-k] */
        A[i] = pow4 + sum_triple;

        /* Compute C2[i] = Σ_{j=0..i} a(j)·a(i-j) for future use */
        seq_t sum_conv = (seq_t)0;
        for (idx_t j = 0; j <= i; ++j) {
            sum_conv += A[j] * A[i - j];
        }
        C2[i] = sum_conv;
    }

    computed = n;
    return A[n];
}
