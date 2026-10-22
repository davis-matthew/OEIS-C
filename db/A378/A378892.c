/*
 * A378892  G.f. A(x) satisfies A(x) = 1 + x*A(x)^6/(1 + x*A(x)^3).
 * https://oeis.org/A378892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   the coefficients a(n) satisfy A(x) = 1 + x*A(x)^6/(1 +
 *            x*A(x)^3), so that a(n) = coeff_{x^{n-1}} (A^6) +
 *            coeff_{x^{n-1}} (A^3) - coeff_{x^{n-1}} (A^4). This is
 *            implemented by maintaining the convolutions
 *            C2(k)=coeff_{x^k}(A^2), C3(k)=coeff_{x^k}(A^3),
 *            C4(k)=coeff_{x^k}(A^4), C6(k)=coeff_{x^k}(A^6...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *A   = NULL;
    static seq_t *C2  = NULL;
    static seq_t *C3  = NULL;
    static seq_t *C4  = NULL;
    static seq_t *C6  = NULL;
    static idx_t  max_n = 0;
    static size_t cap   = 0;

    /* initialise storage if necessary */
    if (cap == 0) {
        cap = 1;                          /* allocate at least one slot */
        A   = (seq_t *)malloc(cap * sizeof(seq_t));
        C2  = (seq_t *)malloc(cap * sizeof(seq_t));
        C3  = (seq_t *)malloc(cap * sizeof(seq_t));
        C4  = (seq_t *)malloc(cap * sizeof(seq_t));
        C6  = (seq_t *)malloc(cap * sizeof(seq_t));
        /* Base values */
        A[0]  = 1;
        C2[0] = 1;   /* 1*1 */
        C3[0] = 1;   /* 1*1*1 */
        C4[0] = 1;   /* 1*1*1*1 */
        C6[0] = 1;   /* 1*1*1*1*1*1 */
    }

    /* increase capacity if needed */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        A   = (seq_t *)realloc(A,   new_cap * sizeof(seq_t));
        C2  = (seq_t *)realloc(C2,  new_cap * sizeof(seq_t));
        C3  = (seq_t *)realloc(C3,  new_cap * sizeof(seq_t));
        C4  = (seq_t *)realloc(C4,  new_cap * sizeof(seq_t));
        C6  = (seq_t *)realloc(C6,  new_cap * sizeof(seq_t));
        /* zero new elements */
        for (size_t i = cap; i < new_cap; ++i) {
            A[i]  = 0;
            C2[i] = 0;
            C3[i] = 0;
            C4[i] = 0;
            C6[i] = 0;
        }
        cap = new_cap;
    }

    /* extend precomputation up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* compute a[m] from previous convolutions */
        seq_t a_val = C6[m - 1] + C3[m - 1] - C4[m - 1];
        A[m] = a_val;

        /* compute C2[m] = coeff of x^m in A^2 */
        seq_t sum2 = 0;
        idx_t half = m / 2;
        for (idx_t i = 0; i <= half; ++i) {
            idx_t j = m - i;
            seq_t prod = A[i] * A[j];
            if (i == j) {
                sum2 += prod;
            } else {
                sum2 += prod + prod;   /* twice the product */
            }
        }
        C2[m] = sum2;

        /* compute C3[m] = coeff of x^m in A^3 = A * (A^2) */
        seq_t sum3 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum3 += A[i] * C2[m - i];
        }
        C3[m] = sum3;

        /* compute C4[m] = coeff of x^m in A^4 = A * (A^3) */
        seq_t sum4 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum4 += A[i] * C3[m - i];
        }
        C4[m] = sum4;

        /* compute C6[m] = coeff of x^m in A^6 = (A^3) * (A^3) */
        seq_t sum6 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum6 += C3[i] * C3[m - i];
        }
        C6[m] = sum6;
    }

    max_n = n;
    return A[n];
}
