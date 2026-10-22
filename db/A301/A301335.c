/*
 * A301335  a(n) = [x^n] 1/(1 + (1/2)*n*(1 - theta_3(x))), where theta_3() is
 *          the Jacobi theta function.
 * https://oeis.org/A301335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k>=1, k^2 <= n} a(n - k^2); a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate space for a[0..n] */
    seq_t *f = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!f) return (seq_t)0;          /* allocation failure */

    f[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m)
    {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k * k <= m; ++k)
            sum += f[m - k * k];

        f[m] = (seq_t)n * sum;
    }
    seq_t ans = f[n];
    free(f);
    return ans;
}
