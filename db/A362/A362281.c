/*
 * A362281  a(n) = n! * Sum_{k=0..floor(n/2)} n^k * binomial(n-k,k)/(n-k)!.
 * https://oeis.org/A362281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} n^k * binomial(n-k,k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t max_k = n / 2;
    seq_t sum = 1;         /* term for k = 0 */
    seq_t term = 1;        /* term for current k */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* term_k = term_{k-1} * (n-2k+2) * (n-2k+1) * n / k */
        term *= (seq_t)(n - 2 * k + 2);
        term *= (seq_t)(n - 2 * k + 1);
        term *= (seq_t)(n);
        term /= (seq_t)k;

        sum += term;
    }

    return sum;
}
