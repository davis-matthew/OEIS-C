/*
 * A264757  An eventually quasi-quadratic solution to Hofstadter's Q
 *          recurrence.
 * https://oeis.org/A264757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=0; thereafter a(6k) = 6k, a(6k+1)=6, a(6k+2)=3,
 *            a(6k+3)=3k^2+3k+5, a(6k+4)=6, a(6k+5)=2
 * Range:     u8 n <= 56, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)0;

    idx_t mod6 = n % 6;
    switch (mod6) {
        case 0:        /* 6k */
            return (seq_t)n;
        case 1:        /* 6k+1 */
            return (seq_t)6;
        case 2:        /* 6k+2 */
            return (seq_t)3;
        case 3: {
            idx_t k = (n - 3) / 6;
            return (seq_t)3 * (seq_t)k * (seq_t)k + (seq_t)3 * (seq_t)k + (seq_t)5;
        }
        case 4:        /* 6k+4 */
            return (seq_t)6;
        case 5:        /* 6k+5 */
            return (seq_t)2;
        default:       /* should never happen */
            return (seq_t)0;
    }
}
