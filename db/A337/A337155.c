/*
 * A337155  a(n) = 5^n * (n!)^2 * Sum_{k=0..n} 1 / ((-5)^k * (k!)^2).
 * https://oeis.org/A337155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n^2*a(n-1) + (-1)^n, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t prev = 1;                /* a(0) */
    if (n == 0) return prev;

    for (size_t i = 1; i <= (size_t)n; ++i) {
        seq_t tn = (seq_t)i;
        seq_t term = prev * 5;          /* prev * 5 */
        term = term * tn * tn;          /* * n * n  */
        if (i & 1) term -= 1;           /* (-1)^i when i is odd */
        else       term += 1;           /* (+1) when i is even */
        prev = term;
    }
    return prev;
}
