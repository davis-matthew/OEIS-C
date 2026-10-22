/*
 * A357673  a(n) = 4*Sum_{k = 0..2*n} binomial(n+k-1,k) + 3*Sum_{k = 0..2*n}
 *          binomial(n+k-1,k)^2.
 * https://oeis.org/A357673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*Sum_{k=0..2*n} binomial(n+k-1,k) + 3*Sum_{k=0..2*n}
 *            binomial(n+k-1,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t limit = 2 * n;
    seq_t sum1 = 0;
    seq_t sum2 = 0;
    seq_t bin   = 1;           /* C(n-1,0) */
    for (idx_t k = 0; k <= limit; ++k) {
        sum1 += bin;
        sum2 += bin * bin;
        if (k == limit)
            break;
        /* next term : C(n-1+(k+1),k+1) = C(n-1+k,k) * (n + k) / (k + 1) */
        seq_t numerator   = (seq_t)(n + k);
        seq_t denominator = (seq_t)(k + 1);
        bin = (bin * numerator) / denominator;
    }
    return 4 * sum1 + 3 * sum2;
}
