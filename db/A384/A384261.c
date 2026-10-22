/*
 * A384261  a(n) = Product_{k=0..n-1} (2*n+k-1).
 * https://oeis.org/A384261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (2*n + k - 1) (equivalently a(n) =
 *            Product_{k=0..n-1} (3*n - 2 - k)) Recurrence: 2*(-2*n+3)*a(n) +
 *            3*(3*n-2)*(3*n-4)*a(n-1) = 0 Offset: a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res   = (seq_t)1;
    seq_t start = (seq_t)(3 * (seq_t)n - 2);   /* 3*n - 2 */
    for (idx_t i = 0; i < n; ++i)
        res *= (start - i);                     /* multiply by 3*n-2-i */

    return res;
}
