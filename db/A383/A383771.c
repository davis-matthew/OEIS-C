/*
 * A383771  Number of noncrossing permutations of [n] avoiding 213 (and by
 *          symmetry 132, 213, or 312).
 * https://oeis.org/A383771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the algebraic equation x^2*A^4 - (x^2+x)*A^3 -
 *            x*A^2 + (x+1)*A - 1 = 0. From this we obtain the recurrence
 *            a(0)=1, for n≥1 a(n) = B3[n-2] + B3[n-1] + B2[n-1] - B4[n-2] -
 *            a(n-1), where B2[n] = Σ_{i=0}^n a[i]*a[n-i] (A^2), B3[n] =
 *            Σ_{i=0}^n a[i]*B2[n-i] (A^3), B4[n] = Σ_{i=0}^n a[i]*...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* Number of non‑crossing 213‑avoiding permutations of [n] */
seq_t a(idx_t n)
{
    /*  Static cache of computed values and convolution sums.
        `size` is the number of terms already computed (next free index).
        `cap` is the allocated capacity. */
    static seq_t *A   = NULL;
    static seq_t *B2  = NULL;
    static seq_t *B3  = NULL;
    static seq_t *B4  = NULL;
    static idx_t  size = 0;
    static idx_t  cap  = 0;

    /* Initialise on first call. */
    if (cap == 0) {
        cap = (n >= 1 ? n+1 : 1);          /* at least one slot       */
        A   = alloc_seq_array(cap);
        B2  = alloc_seq_array(cap);
        B3  = alloc_seq_array(cap);
        B4  = alloc_seq_array(cap);
        /* Base values a(0)=1, and the convolutions for n=0. */
        A[0]  = 1;
        B2[0] = 1;    /* a0*a0 */
        B3[0] = 1;    /* a0*B2[0] */
        B4[0] = 1;    /* a0*B3[0] */
        size   = 1;   /* we have a(0) computed */
    }

    /* Expand cache if the requested n is beyond the current capacity. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *newA  = alloc_seq_array(new_cap);
        seq_t *newB2 = alloc_seq_array(new_cap);
        seq_t *newB3 = alloc_seq_array(new_cap);
        seq_t *newB4 = alloc_seq_array(new_cap);

        for (idx_t i = 0; i < size; ++i) {
            newA[i]  = A[i];
            newB2[i] = B2[i];
            newB3[i] = B3[i];
            newB4[i] = B4[i];
        }

        free(A);  free(B2);  free(B3);  free(B4);
        A = newA; B2 = newB2; B3 = newB3; B4 = newB4;
        cap = new_cap;
    }

    /* Compute missing terms up to n.   */
    for (idx_t k = size; k <= n; ++k) {
        if (k == 0) {                /* a(0) is already known */
            continue;
        }
        /* ---- Compute a(k) from the recurrence ---- */
        seq_t term1 = (k >= 2 ? B3[(size_t)(k-2)] : 0);
        seq_t term2 = B3[(size_t)(k-1)];
        seq_t term3 = B2[(size_t)(k-1)];
        seq_t term4 = (k >= 2 ? B4[(size_t)(k-2)] : 0);
        seq_t term5 = A[(size_t)(k-1)];
        A[(size_t)k] = term1 + term2 + term3 - term4 - term5;

        /* ---- Update convolution sums for the new index k ---- */
        /* B2[k] = Σ a[i]*a[k-i]  */
        seq_t sum2 = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum2 += A[(size_t)i] * A[(size_t)(k - i)];
        }
        B2[(size_t)k] = sum2;

        /* B3[k] = Σ a[i]*B2[k-i]  */
        seq_t sum3 = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum3 += A[(size_t)i] * B2[(size_t)(k - i)];
        }
        B3[(size_t)k] = sum3;

        /* B4[k] = Σ a[i]*B3[k-i]  */
        seq_t sum4 = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum4 += A[(size_t)i] * B3[(size_t)(k - i)];
        }
        B4[(size_t)k] = sum4;
    }

    size = n + 1;             /* we now have a(0)…a(n) cached */
    return A[(size_t)n];
}
