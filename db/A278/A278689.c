/*
 * A278689  a(n) = Sum_{k=0..n} binomial(n+k,n)*binomial(2*n-3,n-k-1) for n>1,
 *          a(n) = n for n<=1.
 * https://oeis.org/A278689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,n)*binomial(2*n-3,n-k-1), a(n)
 *            = n for n <= 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;
    }

    idx_t m = 2 * n - 3;                     /* 2*n-3  for binomial(2*n-3 , ...)  */
    seq_t bin2 = binomial(m, n - 1);         /* binomial(2*n-3, n-1) */
    seq_t bin1 = (seq_t)1;                   /* binomial(n, n) = 1  */
    seq_t acc  = 0;

    for (idx_t k = 0; k < n; ++k) {
        acc += bin1 * bin2;

        if (k + 1 < n) {
            /* update bin1: binomial(n+k+1, n) = bin1 * (n+k+1)/(k+1) */
            bin1 = bin1 * (seq_t)(n + k + 1) / (k + 1);

            /* update bin2: binomial(2*n-3, n-k-2) =
               bin2 * (n-k-1)/(n+k-1) */
            bin2 = bin2 * (seq_t)(n - k - 1) / (seq_t)(n + k - 1);
        }
    }
    return acc;
}
