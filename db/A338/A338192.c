/*
 * A338192  Sum of Fibonacci and tribonacci numbers: a(n) = A000073(n) +
 *          A000045(n).
 * https://oeis.org/A338192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000073(n) + A000045(n) Recurrence: a(n) = 2*a(n-1) +
 *            a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) Initial values: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for n = 0..4 */
    static const seq_t base[5] = {0, 1, 2, 3, 5};

    if (n < 5)
        return base[n];

    /* 5‑element ring buffer for the last five terms */
    seq_t buf[5];
    buf[0] = base[0];
    buf[1] = base[1];
    buf[2] = base[2];
    buf[3] = base[3];
    buf[4] = base[4];

    for (idx_t i = 5; i <= n; ++i) {
        /* next = 2*buf[4] + buf[3] - buf[2] - 2*buf[1] - buf[0] */
        seq_t next = (seq_t)2 * buf[4]
                   + buf[3]
                   - buf[2]
                   - (seq_t)2 * buf[1]
                   - buf[0];

        /* shift buffer left */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }
    return buf[4];
}
