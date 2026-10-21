/*
 * A244281  Answer to Red, Green and Blue Tiles Problem.
 * https://oeis.org/A244281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -9*a(n-2) + 7*a(n-3) -2*a(n-4) -2*a(n-5)
 *            +5*a(n-6) -4*a(n-7) +2*a(n-8) - a(n-9) - a(n-10) + a(n-11)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values a(0)..a(10) */
    static const seq_t init[11] = {
        (seq_t)0,  (seq_t)0,  (seq_t)1,  (seq_t)4,  (seq_t)11, (seq_t)23,
        (seq_t)43, (seq_t)75, (seq_t)126,(seq_t)207,(seq_t)335
    };
    if (n < 11) {
        return init[n];
    }

    /* buffer holding the last 11 terms */
    seq_t buf[11];
    for (idx_t i = 0; i < 11; ++i)
        buf[i] = init[i];

    for (idx_t i = 11; i <= n; ++i) {
        /* recurrence */
        seq_t next = 5 * buf[10] - 9 * buf[9] + 7 * buf[8]
                   - 2 * buf[7] - 2 * buf[6] + 5 * buf[5]
                   - 4 * buf[4] + 2 * buf[3] - buf[2]
                   - buf[1] + buf[0];

        /* shift buffer: discard oldest, shift left, insert new term */
        for (idx_t j = 0; j < 10; ++j)
            buf[j] = buf[j + 1];
        buf[10] = next;
    }

    return buf[10];
}
