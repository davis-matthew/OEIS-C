/*
 * A381206  Expansion of e.g.f. 1/(1 - x*cosh(x))^2.
 * https://oeis.org/A381206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] 1/(1 - x*cosh(x))^2 computed via the auxiliary
 *            sequence h(n) = (k+1)! * A185951(n,k) with h(0) = 1 and
 *            recurrence h(n) = n*h(n−1) + Σ_{m≥1} h(n−2m−1) * C(n,2m) *
 *            (n−2m) Then a(n) = Σ_{k=0..n} h(k) * h(n−k) * C(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Parameters seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays to hold h[0..n] and a[0..n]. */
    seq_t *h  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!h || !a_arr) {
        free(h);
        free(a_arr);
        return (seq_t)0;   /* return 0 on allocation failure */
    }

    h[0] = (seq_t)1;
    a_arr[0] = (seq_t)1;      /* a(0) = 1 */

    /* Compute h[1..n] */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t h_val = h[i - 1] * (seq_t)i;    /* m = 0 term */
        seq_t bin   = (seq_t)1;               /* bin = C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update bin = C(i,k) */
            bin = bin * (seq_t)(i - k + 1) / k;

            /* Use only even k = 2m */
            if ((k & 1) == 0) {
                seq_t coeff = bin * (seq_t)(i - k);
                /* i - k - 1 is the index of h for the term m = k/2 */
                h_val += h[i - k - 1] * coeff;
            }
        }
        h[i] = h_val;
    }

    /* Compute a[0..n] from h using the convolution formula */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_val = h[0] * h[i];             /* k = 0 term, C(i,0) = 1 */
        seq_t bin   = (seq_t)1;                /* bin = C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* bin = C(i,k) */
            bin = bin * (seq_t)(i - k + 1) / k;
            a_val += h[k] * h[i - k] * bin;
        }
        a_arr[i] = a_val;
    }

    seq_t result = a_arr[n];
    free(h);
    free(a_arr);
    return result;
}
