/*
 * A305963  Number of length-n restricted growth strings (RGS) with growth <=
 *          n and fixed first element.
 * https://oeis.org/A305963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * [x^(n-1)] exp(x + Σ_{j=1..n} (exp(j*x)-1)/j) ,
 *            implemented by the recurrence B_1 = n+1, B_k (k≥2) = Σ_{t=1}^n
 *            t^{k-1}, E_0 = 1, E_{m+1} = Σ_{k=0}^m B_{k+1} * C(m,k) * E_{m-k}
 *            (0≤m≤n-2), a(n) = E_{n-1} (for n>0), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Allocate arrays */
    seq_t *B = (seq_t *)calloc((size_t)n + 2, sizeof(seq_t));
    seq_t *E = (seq_t *)calloc((size_t)n,     sizeof(seq_t));
    if (!B || !E) {
        /* In case of memory failure, return 0 (unlikely in tests) */
        free(B);
        free(E);
        return (seq_t)0;
    }

    /* Compute B[1..n] */
    B[1] = (seq_t)1 + (seq_t)n;            /* B_1 = n+1 */
    for (idx_t t = 1; t <= n; ++t) {
        seq_t p = (seq_t)1;                /* t^(k-1) with k starting at 2 */
        for (idx_t k = 2; k <= n; ++k) {
            p *= (seq_t)t;                 /* p = t^(k-1) */
            B[k] += p;                     /* accumulate sum of powers */
        }
    }

    /* Compute E[0..n-1] */
    E[0] = (seq_t)1;
    for (idx_t m = 0; m <= n - 2; ++m) {
        seq_t sum = (seq_t)0;
        seq_t C   = (seq_t)1;              /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            /* term = B[k+1] * C * E[m-k] */
            seq_t term = B[k + 1] * C;
            term *= E[m - k];
            sum += term;
            if (k < m) {
                /* Update C to C(m,k+1) = C * (m-k)/(k+1) */
                C *= (seq_t)(m - k);
                C /= (seq_t)(k + 1);
            }
        }
        E[m + 1] = sum;
    }

    seq_t result = E[n - 1];
    free(B);
    free(E);
    return result;
}
