/*
 * A365854  Expansion of (1/x) * Series_Reversion( x*(1+x)^2*(1-x)^3 ).
 * https://oeis.org/A365854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Sum_{k=0..⌊n/2⌋} binomial(2*n + k + 1 , k) *
 *            binomial(2*n - 2*k , n - 2*k) , offset = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    idx_t maxk = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b = binomial((idx_t)(2 * n + k + 1), k);        /* binomial(2*n + k + 1, k)   */
        seq_t c = binomial((idx_t)(2 * n - 2 * k), (idx_t)(n - 2 * k)); /* binomial(2*n – 2*k, n–2*k) */

        sum += b * c;   /* accumulate term */
    }

    return sum / (seq_t)(n + 1);
}
