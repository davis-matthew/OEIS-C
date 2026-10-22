/*
 * A365027  a(n) = (7*n)!*(13*n/2)!*(n/2)! / ((3*n)!^2 * (7*n/2)!^2 * n!).
 * https://oeis.org/A365027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j = 0..3*n} binomial(7*n, 3*n-j)^2 * binomial(n+j-1,
 *            j)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*  7*n  and 3*n have to fit in an idx_t  */
    idx_t N7 = (idx_t)7 * n;
    idx_t T  = (idx_t)3 * n;           /* upper limit for j (inclusive) */

    /*  binom(7*n, 3*n)  gives the first term for j = 0 */
    seq_t b7 = binomial(N7, T);          /* b7_cur   */
    seq_t b1 = 1;                        /* binomial(n-1,0) = 1   */

    seq_t sum = 0;

    for (idx_t j = 0; j <= T; ++j)
    {
        /* add current term to the sum */
        seq_t prod = b7 * b7;
        prod = prod * b1;
        sum = sum + prod;

        /* no further update on the last iteration */
        if (j == T)
            break;

        /* update b7 for the next j (t reduces by 1) */
        idx_t t = T - j;                 /* current 3*n - j  */
        seq_t numerator_b7  = (seq_t)t;
        seq_t denominator_b7= (seq_t)(N7 - t + 1);
        seq_t tmp = b7 * numerator_b7;
        b7 = tmp / denominator_b7;

        /* update b1 for the next j */
        seq_t numerator_b1  = (seq_t)(n + j);
        seq_t denominator_b1= (seq_t)(j + 1);
        tmp = b1 * numerator_b1;
        b1 = tmp / denominator_b1;
    }

    return sum;
}
