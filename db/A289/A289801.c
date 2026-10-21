/*
 * A289801  p-INVERT of the tetrahedral numbers (A000292), where p(S) = 1 - S
 *          - S^2.
 * https://oeis.org/A289801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) obeys the recurrence a(n) = 9*a(n-1) - 31*a(n-2) +
 *            62*a(n-3) - 74*a(n-4) + 57*a(n-5) - 28*a(n-6) + 8*a(n-7) -
 *            a(n-8) with initial values a(0)=1, a(1)=6, a(2)=29, a(3)=133,
 *            a(4)=597, a(5)=2661, a(6)=11856, a(7)=52878
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 8 terms */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)6, (seq_t)29, (seq_t)133,
        (seq_t)597, (seq_t)2661, (seq_t)11856, (seq_t)52878
    };

    /* handle trivial cases */
    if (n < 8)
        return init[(size_t)n];

    /* buffer to hold the last 8 computed terms */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    idx_t i;
    for (i = 8; i <= n; ++i) {
        /* indices of previous terms in the buffer (mod 8) */
        seq_t a1 = buf[(i - 1) % 8];
        seq_t a2 = buf[(i - 2) % 8];
        seq_t a3 = buf[(i - 3) % 8];
        seq_t a4 = buf[(i - 4) % 8];
        seq_t a5 = buf[(i - 5) % 8];
        seq_t a6 = buf[(i - 6) % 8];
        seq_t a7 = buf[(i - 7) % 8];
        seq_t a8 = buf[(i - 8) % 8];

        buf[i % 8] =
            9 * a1 - 31 * a2 + 62 * a3 - 74 * a4
          + 57 * a5 - 28 * a6 + 8 * a7 - a8;
    }

    return buf[n % 8];
}
