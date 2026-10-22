/*
 * A395782  Expansion of (Sum_{k>=0} binomial(4*k+1,k) * x^k)^4.
 * https://oeis.org/A395782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395782:
   a(n) = Sum_{k=0..n} 4^k * binomial(k+2,2) * binomial(4*n+4-k, n-k) */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    /* B_k = binomial(4*n+4-k, n-k), start with k = 0 */
    seq_t B   = binomial(4*n + 4, n);
    seq_t pow4 = 1;               /* 4^k, start at k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t c2 = (seq_t)(k + 2) * (k + 1) / 2;

        /* term = 4^k * c2 * B_k */
        sum += pow4 * c2 * B;

        if (k == n) break;        /* last term processed */

        /* Update B for next k: B_{k+1} = B_k * (n-k) / (4*n+4-k) */
        B = B * (n - k) / (seq_t)(4*n + 4 - k);

        /* Update pow4: 4^(k+1) = 4^k * 4 */
        pow4 *= 4;
    }

    return sum;
}
