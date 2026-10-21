/*
 * A291240  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S^3)^2.
 * https://oeis.org/A291240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + 2*a(n-3) - 15*a(n-4) - 6*a(n-5) + 19*a(n-6) +
 *            6*a(n-7) - 15*a(n-8) - 2*a(n-9) + 6*a(n-10) - a(n-12),
 *            a(0)=0,a(1)=0,a(2)=2,
 *            a(3)=0,a(4)=6,a(5)=3,a(6)=12,a(7)=18,a(8)=24,a(9)=63,a(10)=66,
 *            a(11)=173,a(12)=222
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 44, u64 n <= 89, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Initial values for n = 0 .. 12. */
    static const seq_t init[13] = {
        0, 0, 2, 0, 6, 3, 12, 18, 24, 63, 66, 173, 222
    };

    if (n < 13) {
        return init[(idx_t)n];
    }

    /* Use a circular buffer of size 13 to keep the last values needed
       by the recurrence. */
    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i) {
        buf[i] = init[(idx_t)i];
    }

    for (idx_t i = 13; i <= n; ++i) {
        seq_t val =  6 * buf[(i - 2) % 13]
                   + 2 * buf[(i - 3) % 13]
                   - 15 * buf[(i - 4) % 13]
                   - 6  * buf[(i - 5) % 13]
                   + 19 * buf[(i - 6) % 13]
                   + 6  * buf[(i - 7) % 13]
                   - 15 * buf[(i - 8) % 13]
                   - 2  * buf[(i - 9) % 13]
                   + 6  * buf[(i -10) % 13]
                   -     buf[(i -12) % 13];
        buf[i % 13] = val;
    }

    return buf[n % 13];
}
