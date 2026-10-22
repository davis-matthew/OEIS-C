/*
 * A359066  a(n) = Sum_{k=0..floor((n-1)/2)}
 *          binomial(n,k)*binomial(n-1-k,floor((n-1)/2) - k).
 * https://oeis.org/A359066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)}
 *            binomial(n,k)*binomial(n-1-k,floor((n-1)/2)-k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* m = floor((n-1)/2) */
    idx_t m = (n - 1) / 2;

    /* Current binomial coefficients:
       C = binomial(n, k)
       D = binomial(n-1-k, m-k)   (initially for k=0)     */
    seq_t C = 1;                           /* binomial(n, 0) = 1 */
    seq_t D = binomial(n - 1, m);          /* binomial(n-1, m)   */

    seq_t sum = 0;

    for (idx_t k = 0; k <= m; ++k) {
        sum += C * D;

        if (k == m) break;                 /* last term processed */

        /* Update C to binomial(n, k+1) */
        C = C * ( (seq_t)(n - k) ) / ( (seq_t)(k + 1) );

        /* Update D to binomial(n-1-(k+1), m-(k+1)) */
        D = D * ( (seq_t)(m - k) ) / ( (seq_t)(n - 1 - k) );
    }

    return sum;
}
