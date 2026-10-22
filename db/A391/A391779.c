/*
 * A391779  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(k+6,6) *
 *          binomial(2*k,2*(n-k)).
 * https://oeis.org/A391779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(k+6,6) *
 *            binomial(2*k, 2*(n-k))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow2k = 1;          /* 2^k   - start with k = 0 */
    seq_t pow3m = pow3(n);    /* 3^(n-k) - start with k = 0  */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial(2*k, 2*(n-k)) is zero when 2*(n-k) > 2*k */
        if (2 * (n - k) <= 2 * k)
        {
            seq_t term = pow2k * pow3m;
            term *= binomial(k + 6, 6);
            term *= binomial(2 * k, 2 * (n - k));
            sum += term;
        }

        /* update powers for next iteration */
        pow2k *= 2;
        if (k < n)           /* avoid division after the last term */
            pow3m /= 3;
    }

    return sum;
}
