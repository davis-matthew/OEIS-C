/*
 * A368793  a(n) = (3*n-2)!!! * Sum_{k=0..n} k/(3*k-2)!!!.
 * https://oeis.org/A368793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(n)=(3*n-2)*a(n-1)+n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t prev = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ti = (seq_t)i;
        seq_t coeff = ( (seq_t)3 * ti ) - (seq_t)2;
        prev = coeff * prev + ti;
    }
    return prev;
}
