/*
 * A391516  Expansion of 1/(g * (2 - g^2)), where g = 1+x*g^2 is the g.f. of
 *          A000108.
 * https://oeis.org/A391516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*n)) * Sum_{k=1..n} k * ((-1)^k + Pell(k) +
 *            Pell(k+1)) * binomial(2*n, n-k) (with a(0) = 1) where Pell(0)=0,
 *            Pell(1)=1, Pell(k)=2*Pell(k-1)+Pell(k-2) for k≥2. The function
 *            implements this directly, computing Pell numbers and binomial
 *            coefficients on the fly; it requires no global stat...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   compute_pell_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_pell_array.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for Pell numbers up to n+1 */
    idx_t pell_len = n + 2;  /* need indices 0..n+1 inclusive */
    seq_t *pell = (seq_t *)malloc(pell_len * sizeof(seq_t));
    if (!pell)
        abort();

    compute_pell_array(pell, n + 1);

    /* Compute binomial(2*n, n) multiplicatively in seq_t */
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        bin = bin * (n + i) / i;
    }

    seq_t sum = (seq_t)0;
    /* Iterate k = 1..n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update bin to C(2*n, n-k) for this k:
           bin_{new} = bin_{old} * (n - k + 1) / (n + k)   */
        bin = bin * (n - k + 1) / (n + k);

        /* t = Pell(k) + Pell(k+1) + (-1)^k */
        seq_t t = pell[k] + pell[k + 1];
        if ((k & 1) == 0) /* even k -> +1 */
            t = t + (seq_t)1;
        else             /* odd k  -> -1 */
            t = t - (seq_t)1;

        /* term = k * t * bin */
        seq_t term = t * (seq_t)k * bin;
        sum = sum + term;
    }

    /* Final division by 2*n */
    seq_t denom = (seq_t)2 * (seq_t)n;
    seq_t result = sum / denom;

    free(pell);
    return result;
}
