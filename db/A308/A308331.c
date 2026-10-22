/*
 * A308331  a(n) = n! * [x^n] exp(exp(n*x)/(1 + x) - 1).
 * https://oeis.org/A308331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(exp(n*x)/(1 + x) - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    size_t sz = (size_t)n + 1;

    /* Allocate temporary arrays */
    seq_t *d = seq_alloc_new_array(sz);   /* d[k] = k! * coefficient of x^k in exp(n*x)/(1+x) */
    seq_t *B = seq_alloc_new_array(sz);   /* Bell polynomial values B_m for m<=n */

    /* Compute d[0..n] using recurrence:
       d_{k+1} = n^{k+1} - (k+1) * d_k ,  d_0 = 1                */
    d[0] = 1;
    seq_t pow_val = 1;                  /* n^0 initially */
    for (idx_t k = 0; k < n; ++k) {
        pow_val = pow_val * (seq_t)n;    /* n^{k+1} */
        d[k + 1] = pow_val - ((seq_t)(k + 1) * d[k]);
    }

    /* Compute Bell numbers B_m with recurrence:
       B_0 = 1
       B_m = Σ_{k=1..m} C(m-1, k-1) * d_k * B_{m-k}              */
    B[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;                  /* bin = C(m-1, 0) */
        for (idx_t k = 1; k <= m; ++k) {
            sum += bin * d[k] * B[m - k];
            if (k < m) {
                /* Update bin to C(m-1, k) from C(m-1, k-1) */
                bin = bin * (m - k) / k;
            }
        }
        B[m] = sum;
    }

    seq_t result = B[n];
    seq_free_array(d);
    seq_free_array(B);
    return result;
}
