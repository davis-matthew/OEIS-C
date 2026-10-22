/*
 * A383716  a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(4*n-k,n-k).
 * https://oeis.org/A383716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(4*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Initial term: k = 0
     * t_0 = binomial(4*n+1,0) * binomial(4*n-0,n-0) = binomial(4*n,n)   */
    seq_t term = binomial(4 * n, n);

    for (idx_t k = 0; k <= n; ++k) {
        sum += term;
        if (k == n)   /* last term added; exit loop */
            break;

        /* Compute t_{k+1} from t_k using
         *   t_{k+1} = t_k * (n-k)/(4*n-k) * (4*n+1-k)/(k+1)    */
        term *= (seq_t)(n - k);
        term /= (seq_t)(4 * n - k);
        term *= (seq_t)(4 * n + 1 - k);
        term /= (seq_t)(k + 1);
    }

    return sum;
}
