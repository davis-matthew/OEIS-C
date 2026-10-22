/*
 * A368688  Expansion of g.f. Sum_{n>=1} q^n/(1-q^n-q^(3*n)).
 * https://oeis.org/A368688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} f(d), where f(1)=f(2)=f(3)=1 and
 *            f(k+3)=f(k+2)+f(k) for k>=1
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    static seq_t *f = NULL;
    static seq_t *sum = NULL;

    if (n == 0)
        return (seq_t)0;            /* defined for safety */

    if (n > max_n) {
        /* Resize arrays to accommodate up to n. */
        idx_t new_size = n + 1;     /* indices 0..n inclusive */
        seq_t *f_new = (seq_t *)realloc(f, new_size * sizeof(seq_t));
        if (!f_new) return (seq_t)0;/* out of memory: unlikely    */
        f = f_new;

        seq_t *sum_new = (seq_t *)realloc(sum, new_size * sizeof(seq_t));
        if (!sum_new) return (seq_t)0;
        sum = sum_new;

        /* Compute f(i) for i = max_n+1 .. n. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1 || i == 2 || i == 3)
                f[i] = (seq_t)1;
            else
                f[i] = f[i-1] + f[i-3];
        }

        /* Zero the contribution array. */
        for (idx_t i = 0; i <= n; ++i)
            sum[i] = (seq_t)0;

        /* Compute the divisor sums for a(i) (Dirichlet convolution). */
        for (idx_t d = 1; d <= n; ++d) {
            for (idx_t m = d; m <= n; m += d)
                sum[m] += f[d];
        }

        max_n = n;
    }

    return sum[n];
}
