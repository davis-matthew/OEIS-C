/*
 * A386210  G.f. A(x) satisfies A(x) = 1/(1-x)^3 + x^2 * (d/dx A(x)^2).
 * https://oeis.org/A386210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+2,2) + (n-1)*Sum_{k=0..n-1} a(k)*a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoize previously computed terms */
    static seq_t *arr   = NULL;   /* array of a[0 .. cache_size-1] */
    static idx_t  cache_size = 0; /* number of stored terms */

    /* expand cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_size * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        arr = tmp;

        /* initial term a(0) */
        if (cache_size == 0) {
            arr[0] = 1;
            cache_size = 1;
        }

        /* compute any missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t m = i - 1;                /* used only for the convolution sum */

            /* Σ_{k=0}^{m} a(k) * a(m-k) */
            seq_t sum = 0;
            for (idx_t k = 0; k <= m; ++k) {
                sum += arr[k] * arr[m - k];
            }

            /* binomial(i+2,2) = (i+2)*(i+1)/2 */
            seq_t bin = ((seq_t)(i + 2)) * ((seq_t)(i + 1)) / 2;

            /* a(i) = bin + (i-1) * sum */
            arr[i] = bin + ((seq_t)i - 1) * sum;
        }

        cache_size = new_size;
    }

    return arr[n];
}
