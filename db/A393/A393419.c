/*
 * A393419  Expansion of (1/x) * Series_Reversion( x * (3 * (1 - x)^2 - 2) ).
 * https://oeis.org/A393419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393419: a(0)=1; for n>=1
   a(n) = 6*∑_{i=0}^{n-1} a(i)*a(n-1-i)
          - 3*∑_{i=0}^{n-2} a(i)*∑_{j=0}^{n-2-i} a(j)*a(n-2-i-j)  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static state to memoise computed terms */
    static seq_t *a_arr      = NULL; /* array of a values */
    static seq_t *c2_arr    = NULL; /* array of coeff(A^2, m) values */
    static idx_t  cap       = 0;     /* current allocation size   */
    static idx_t  last_comp = 0;     /* highest index computed so far (initially 0) */

    /* Allocate arrays if first call or too small for requested n */
    if (a_arr == NULL) {
        cap     = n + 1;
        a_arr   = (seq_t *)malloc(cap * sizeof(seq_t));
        c2_arr  = (seq_t *)malloc(cap * sizeof(seq_t));
        a_arr[0] = (seq_t)1;
        c2_arr[0] = (seq_t)1;
        last_comp = 0;
    } else if (n + 1 > cap) {
        cap     = n + 1;
        a_arr   = (seq_t *)realloc(a_arr, cap * sizeof(seq_t));
        c2_arr  = (seq_t *)realloc(c2_arr, cap * sizeof(seq_t));
    }

    /* Return the value if already computed */
    if (n <= last_comp)
        return a_arr[n];

    /* Compute new terms from last_comp+1 up to n */
    for (idx_t k = last_comp + 1; k <= n; ++k) {
        /* Compute s3 = coeff(A^3, k-2) */
        seq_t s3 = (seq_t)0;
        if (k >= 2) {
            idx_t m = k - 2;
            for (idx_t i = 0; i <= m; ++i)
                s3 += a_arr[i] * c2_arr[m - i];
        }

        /* a[k] = 6*c2_arr[k-1] - 3*s3 */
        a_arr[k] = ((seq_t)6 * c2_arr[k - 1]) - ((seq_t)3 * s3);

        /* Compute c2_arr[k] = coeff(A^2, k) */
        seq_t c2k = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c2k += a_arr[i] * a_arr[k - i];
        c2_arr[k] = c2k;
    }

    last_comp = n;
    return a_arr[n];
}
