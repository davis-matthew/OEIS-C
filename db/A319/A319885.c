/*
 * A319885  a(n) = 2*1 - 4*3 + 6*5 - 8*7 + 10*9 - 12*11 + 14*13 - 16*15 + ...
 *          - (up to the n-th term).
 * https://oeis.org/A319885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (order‑7 linear recurrence, offset 1):
 *   a(1)=  2,  a(2)=  2,  a(3)= -2,  a(4)= -10,  a(5)=  -4,
 *   a(6)= 20,  a(7)= 12
 *   a(n) = a(n-1) - 3*a(n-2) + 3*a(n-3) - 3*a(n-4) + 3*a(n-5) - a(n-6) + a(n-7)
 * for all n > 7.
 */

#include <stdint.h>  /* for uint64_t definition of idx_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the first seven terms directly */
    if (n <= 7) {
        static const seq_t init[7] = { 2, 2, -2, -10, -4, 20, 12 };
        return init[n - 1];
    }

    /* Sliding window of the last seven computed values:
     * wnd[0] = a(k-6), wnd[1] = a(k-5), … , wnd[6] = a(k-1)  (for k starting at 8)
     */
    seq_t wnd[7];
    wnd[0] =  2;  /* a(1) */
    wnd[1] =  2;  /* a(2) */
    wnd[2] = -2;  /* a(3) */
    wnd[3] = -10; /* a(4) */
    wnd[4] = -4;  /* a(5) */
    wnd[5] = 20;  /* a(6) */
    wnd[6] = 12;  /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = wnd[6]          /* a(i-1)   */
                   - 3 * wnd[5]    /* -3*a(i-2)*/
                   + 3 * wnd[4]    /* +3*a(i-3)*/
                   - 3 * wnd[3]    /* -3*a(i-4)*/
                   + 3 * wnd[2]    /* +3*a(i-5)*/
                   - wnd[1]        /* -a(i-6) */
                   + wnd[0];       /* +a(i-7) */

        /* Shift the window left by one position */
        wnd[0] = wnd[1];
        wnd[1] = wnd[2];
        wnd[2] = wnd[3];
        wnd[3] = wnd[4];
        wnd[4] = wnd[5];
        wnd[5] = wnd[6];
        wnd[6] = next;
    }

    return wnd[6];  /* a(n) */
}
