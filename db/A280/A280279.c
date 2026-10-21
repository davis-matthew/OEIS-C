/*
 * A280279  Number of n X 1 0..2 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4) for n > 7 * Initial
 *            values: a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=8, * a(6)=28,
 *            a(7)=94
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n < 1) return 0;

    /* Small n are returned directly */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 2;
    if (n == 5) return 8;
    if (n == 6) return 28;
    if (n == 7) return 94;

    /* For n >= 8 compute by recurrence, keeping the last four terms.
       t0 = a(n-4), t1 = a(n-3), t2 = a(n-2), t3 = a(n-1)                */
    seq_t t0 = 2;   /* a(4)  */
    seq_t t1 = 8;   /* a(5)  */
    seq_t t2 = 28;  /* a(6)  */
    seq_t t3 = 94;  /* a(7)  */
    seq_t next;
    idx_t i = 8;

    while (i <= n) {
        /* a(i) = 4*a(i-1) - 8*a(i-3) - 4*a(i-4) */
        next = 4 * t3;
        next -= 8 * t1;
        next -= 4 * t0;

        /* shift the window for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;

        ++i;
    }

    return t3;  /* a(n) is now in t3 */
}
