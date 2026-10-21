/*
 * A299594  Number of n X 7 0..1 arrays with every element unequal to 0, 1 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A299594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) - a(n-6) for
 *            n>9 with initial values a(1)=21, a(2)=26, a(3)=65, a(4)=250,
 *            a(5)=541, a(6)=1225, a(7)=2583, a(8)=5488, a(9)=11899
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* initial 9 terms */
    static const seq_t base[9] = {
        (seq_t)21, (seq_t)26, (seq_t)65, (seq_t)250, (seq_t)541,
        (seq_t)1225, (seq_t)2583, (seq_t)5488, (seq_t)11899
    };

    if (n <= 9) {
        return base[n - 1];
    }

    seq_t a1 = base[8]; /* a(n-1) */
    seq_t a2 = base[7]; /* a(n-2) */
    seq_t a3 = base[6]; /* a(n-3) */
    seq_t a4 = base[5]; /* a(n-4) */
    seq_t a5 = base[4]; /* a(n-5) */
    seq_t a6 = base[3]; /* a(n-6) */
    seq_t next;

    for (idx_t k = 10; k <= n; ++k) {
        next = a1 + a2 + 3 * a3 + a4 - a5 - a6;
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    return next;
}
