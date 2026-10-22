/*
 * A381056  Product of row n of A329708.
 * https://oeis.org/A381056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..2*n} Sum_{i=max(0,k-n)}^{min(k,n)}
 *            (i+1)*(k-i+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   term_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/term_sum.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t maxk = 2 * n;
    for (idx_t k = 0; k <= maxk; ++k)
        prod *= term_sum(n, k);
    return prod;
}
