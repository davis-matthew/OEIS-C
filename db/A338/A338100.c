/*
 * A338100  Number of spanning trees in the n X 2 king graph.
 * https://oeis.org/A338100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(n-2) * 16 for n >= 2, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, but safe */
    
    if (n == 1)
        return (seq_t)1;
    
    /* For n >= 2: a(n) = 12^(n-2) * 16 */
    seq_t pow12 = big_pow((seq_t)12, (seq_t)(n - 2));
    return pow12 * (seq_t)16;
}
