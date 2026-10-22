/*
 * A366456  G.f. A(x) satisfies A(x) = 1 + x + x/A(x)^(7/2).
 * https://oeis.org/A366456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * Sum_{k=0..n} binomial(9*k/2-1,k) *
 *            binomial(n+7*k/2-2,n-k) / (9*k/2-1) (OEIS A366456)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static storage for memoised coefficients a[0..] and b[0..] where b[i] = A^(7/2) coefficient */
    static idx_t  computed = 0;            /* number of computed terms (max index) */
    static size_t cap      = 0;            /* current capacity of the arrays */
    static seq_t *A = NULL;                /* a coefficients */
    static seq_t *B = NULL;                /* b coefficients (coefficient of A^(7/2) ) */

    /* Ensure the arrays are large enough and initialise for the first time */
    if (cap == 0) {
        cap = 2;
        A = (seq_t *)malloc(cap * sizeof(seq_t));
        B = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!A || !B) {
            /* Allocation failed; return 0 as a fallback */
            return (seq_t)0;
        }
        /* a(0)=1, a(1)=2  ;  b[0]=1 (A^0), b[1]=7  (coefficient of A^(7/2) for x^1) */
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        B[0] = (seq_t)1;
        B[1] = (seq_t)7;
        computed = 1;
    }

    /* Extend the precomputation if needed */
    if ((idx_t)computed < n) {
        idx_t last = computed;                 /* last computed index */
        /* Grow to reach at least n */
        if ((size_t)n+1 > cap) {
            size_t new_cap = (size_t)n + 1;
            seq_t *newA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            seq_t *newB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
            if (!newA || !newB) {
                /* Allocation failed; return 0 as a fallback */
                return (seq_t)0;
            }
            A = newA;
            B = newB;
            cap = new_cap;
        }

        /* Compute new terms from last+1 up to n */
        for (idx_t k = (idx_t)computed + 1; k <= n; ++k) {
            /* ----- Compute a[k] -----
               a_k = - ( B_{k-1} + Σ_{m=2}^{k-1} a_m * B_{k-m} )  */
            seq_t sum = B[(size_t)(k - 1)];
            for (idx_t m = 2; m <= k - 1; ++m) {
                sum += A[(size_t)m] * B[(size_t)(k - m)];
            }
            A[(size_t)k] = -sum;

            /* ----- Compute b[k] -----
               Using
               b_k = ( 7*k*a_k + 7*Σ_{j=1}^{k-1} j*B_{k-j}*A_j
                         - 2*Σ_{j=1}^{k-1} j*A_{k-j}*B_j ) / (2*k)  */
            seq_t S1 = (seq_t)0;
            seq_t S2 = (seq_t)0;
            for (idx_t j = 1; j <= k - 1; ++j) {
                seq_t prod1 = (seq_t)j * B[(size_t)(k - j)] * A[(size_t)j];
                seq_t prod2 = (seq_t)j * A[(size_t)(k - j)] * B[(size_t)j];
                S1 += prod1;
                S2 += prod2;
            }
            seq_t term1 = (seq_t)7 * (seq_t)k * A[(size_t)k];
            seq_t term2 = (seq_t)7 * S1;
            seq_t term3 = (seq_t)2 * S2;
            seq_t numer  = term1 + term2 - term3;
            seq_t denom  = (seq_t)2 * (seq_t)k;
            B[(size_t)k] = numer / denom;   /* division is exact */
        }

        computed = n;
    }

    return A[(size_t)n];
}
