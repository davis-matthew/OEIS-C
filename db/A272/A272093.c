/*
 * A272093  a(n) = Product_{k=0..n} binomial(k*n,k).
 * https://oeis.org/A272093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} binomial(k*n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = (seq_t)1;
        idx_t base   = k * n;          /* k*n */
        idx_t offset = base - k;       /* (k*n) - k */
        for (idx_t i = 1; i <= k; ++i)
        {
            term = term * (seq_t)(offset + i) / (seq_t)i;
        }
        result *= term;
    }
    return result;
}
