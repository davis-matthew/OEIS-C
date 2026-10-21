/*
 * A250880  Number of (3+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2002, a(2)=6812, a(3)=16262, a(4)=31928; For n>4:
 *            a(n)=4*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4)
 * Range:     u8 none, u16 n <= 5, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    /* initial terms */
    seq_t a1 = (seq_t)2002;
    seq_t a2 = (seq_t)6812;
    seq_t a3 = (seq_t)16262;
    seq_t a4 = (seq_t)31928;
    seq_t an;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* iterative computation using the 4‑term recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        an = 4 * a4 - 6 * a3 + 4 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
