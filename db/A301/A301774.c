/*
 * A301774  Number of odd chordless cycles in the (2n+1)-prism graph.
 * https://oeis.org/A301774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4); Initial values:
 *            a(1)=2, a(2)=12, a(3)=30, a(4)=74
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 12;
    if (n == 3) return 30;
    if (n == 4) return 74;

    seq_t a1 = 2, a2 = 12, a3 = 30, a4 = 74;
    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = 2 * a4 + a3 + 2 * a2 - a1;
        a1 = a2; a2 = a3; a3 = a4; a4 = an;
    }
    return a4;
}
