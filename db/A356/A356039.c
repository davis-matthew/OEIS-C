/*
 * A356039  a(n) = Sum_{k=1..n} binomial(n,k) * sigma_3(k).
 * https://oeis.org/A356039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * sigma_3(k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;                     /* binomial(n,0) */
    seq_t n_seq = (seq_t)n;            /* convert n once */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(n,k) = binomial(n,k-1) * (n-k+1) / k   */
        seq_t factor = n_seq - (seq_t)k + 1;
        bin = (bin * factor) / (seq_t)k;

        /* sigma_3(k) = sum of d^3 over all divisors d of k */
        seq_t sigma = sigma_k(k, 3);

        sum += bin * sigma;
    }
    return sum;
}
