/*
 * A386833  a(n) = Sum_{k=0..n} binomial(3*n+1,k) * binomial(3*n-k-1,n-k).
 * https://oeis.org/A386833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+1,k) * binomial(3*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* term for k = 0: binomial(3*n+1,0) * binomial(3*n-1,n) */
    const idx_t top = 3 * n - 1;
    seq_t term = binomial(top, n);
    seq_t sum  = term;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t num1 = (seq_t)(3 * n + 2 - k);
        seq_t den1 = (seq_t)k;
        seq_t num2 = (seq_t)(n - k + 1);
        seq_t den2 = (seq_t)(3 * n - k);

        term = term * num1 / den1;   /* exact division */
        term = term * num2 / den2;   /* exact division */

        sum += term;
    }

    return sum;
}
