/*
 * A394949  Number of equivalence classes of well-formed bracketed words of
 *          total length 2n built from the symbol * (of length 2) and three
 *          unary bracket types (), [], and {}, with no empty bracket pair,
 *          modulo commutativity of concatenation.
 * https://oeis.org/A394949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1; for n>1,
   n*a(n)=c(n)+∑_{k=1}^{n-1}c(k)*a(n-k)
   with c(n)=∑_{d|n} d*b(d),
   b(1)=1, b(n)=3*a(n-1). */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *A = NULL;
    static seq_t *C = NULL;
    static size_t cap = 0;          /* current allocated capacity */
    static size_t computed = 0;     /* index of next value to compute */

    /* First call: initialise arrays */
    if (A == NULL) {
        size_t init_cap = (size_t)n + 1;
        if (init_cap == 0) init_cap = 1;
        A = (seq_t*)malloc(init_cap * sizeof(seq_t));
        C = (seq_t*)malloc(init_cap * sizeof(seq_t));
        for (size_t i = 0; i < init_cap; ++i) {
            A[i] = (seq_t)0;
            C[i] = (seq_t)0;
        }
        A[0] = (seq_t)1;
        if (init_cap > 1) {
            A[1] = (seq_t)1;
        }
        cap = init_cap;
        computed = (init_cap > 1) ? 2 : 1;
    }

    /* Grow arrays if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        C = (seq_t*)realloc(C, new_cap * sizeof(seq_t));
        for (size_t i = cap; i < new_cap; ++i) {
            A[i] = (seq_t)0;
            C[i] = (seq_t)0;
        }
        cap = new_cap;
    }

    /* Compute values up to n if not yet done */
    if ((size_t)n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            /* Compute c(i) */
            seq_t ci = (seq_t)0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t e = i / d;
                    seq_t b_d = (d == 1) ? (seq_t)1 : (seq_t)3 * A[d - 1];
                    ci += (seq_t)d * b_d;
                    if (e != d) {
                        seq_t b_e = (e == 1) ? (seq_t)1 : (seq_t)3 * A[e - 1];
                        ci += (seq_t)e * b_e;
                    }
                }
            }
            C[i] = ci;

            /* Convolution sum Σ_{k=1}^{i-1} C[k]*A[i-k] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                sum += C[k] * A[i - k];
            }

            /* Compute a(i) using the recurrence */
            if (i <= 1) {
                A[i] = (seq_t)1;          /* already set but keeps logic clear */
            } else {
                A[i] = (ci + sum) / (seq_t)i;
            }
        }
        computed = n + 1;
    }

    return A[n];
}
