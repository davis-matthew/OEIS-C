/*
 * A294627  Expansion of x*(1 + x)/((1-2*x)*(1+x+x^2)).
 * https://oeis.org/A294627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3), a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    seq_t a0 = 0, a1 = 1, a2 = 2, a3;
    for (idx_t i = 3; i <= n; ++i) {
        a3 = a2 + a1 + 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
