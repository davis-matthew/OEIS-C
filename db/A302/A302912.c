/*
 * A302912  Determinant of n X n matrix whose main diagonal consists of the
 *          first n 8-gonal numbers and all other elements are 1's.
 * https://oeis.org/A302912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=a(n-1)*(n-1)*(3*(n-1)+4)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t k = (seq_t)(i - 1);                    /* k = n-1 */
        seq_t f = k * ((seq_t)3 * k + (seq_t)4);     /* f = k*(3k+4) */
        prod = prod * f;
    }
    return prod;
}
