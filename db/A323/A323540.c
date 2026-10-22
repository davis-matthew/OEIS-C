/*
 * A323540  a(n) = Product_{k=0..n} (k^2 + (n-k)^2).
 * https://oeis.org/A323540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (k^2 + (n-k)^2) (OEIS A323540)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* For uint64_t if needed; seq_t may be typedef'd elsewhere */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t n_seq = (seq_t)n;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t k = (seq_t)i;
        seq_t diff = n_seq - k;
        seq_t term = k * k + diff * diff;
        prod *= term;
    }
    return prod;
}
