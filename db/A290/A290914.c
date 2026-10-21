/*
 * A290914  a(n) = (1/7)*A290913(n).
 * https://oeis.org/A290914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) + 4*a(n-3) - a(n-4); Initial values:
 *            a(0)=0, a(1)=1, a(2)=4, a(3)=17
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)17;

    seq_t a0 = (seq_t)0, a1 = (seq_t)1, a2 = (seq_t)4, a3 = (seq_t)17, a4;
    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)4 * a3 + a2 + (seq_t)4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
