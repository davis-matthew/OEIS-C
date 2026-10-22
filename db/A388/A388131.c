/*
 * A388131  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(n,k) * binomial(4*n+1,k).
 * https://oeis.org/A388131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(n,k) * binomial(4*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* (4*n + 1) fits in idx_t for the required range */
    const idx_t four_n_plus_one = 4 * n + 1;

    seq_t sum   = 0;
    seq_t power = pow2(n);          /* 2^n */
    seq_t bin1  = 1;                /* binomial(n, 0)   */
    seq_t bin2  = 1;                /* binomial(4n+1, 0)*/

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 2^(n-k) * binomial(n,k) * binomial(4*n+1,k) */
        sum += power * bin1 * bin2;

        if (k == n) {               /* last iteration, exit loop */
            break;
        }

        /* Update bin1 to binomial(n, k+1) */
        seq_t num1 = (seq_t)(n - k);
        seq_t den  = (seq_t)(k + 1);
        bin1 = (bin1 * num1) / den;

        /* Update bin2 to binomial(4n+1, k+1) */
        seq_t num2 = (seq_t)(four_n_plus_one - k);
        bin2 = (bin2 * num2) / den;

        /* 2^(n-(k+1)) = 2^(n-k) / 2 */
        power = power / 2;
    }

    return sum;
}
