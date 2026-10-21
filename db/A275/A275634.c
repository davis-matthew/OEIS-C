/*
 * A275634  Expansion of ( 3-2*x-2*x^2 ) / ( 1-5*x+2*x^2+3*x^3 ).
 * https://oeis.org/A275634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-2*a(n-2)-3*a(n-3), a(0) = 3, a(1) = 13, a(2) =
 *            57
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)57;

    seq_t a0 = (seq_t)3;
    seq_t a1 = (seq_t)13;
    seq_t a2 = (seq_t)57;
    seq_t a3 = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 5 * a2 - 2 * a1 - 3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a3;
}
