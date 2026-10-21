/*
 * A299285  Coordination sequence for "tea" 3D uniform tiling.
 * https://oeis.org/A299285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A299285: Coordination sequence for "tea" 3D uniform tiling
   Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6)
   with initial values:
   a(0)=1  a(1)=10  a(2)=33  a(3)=73  a(4)=128  a(5)=199  a(6)=285
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 7‑term static table of the initial values */
    static const seq_t init[7] = { (seq_t)1,  (seq_t)10,  (seq_t)33,
                                   (seq_t)73,  (seq_t)128, (seq_t)199,
                                   (seq_t)285 };

    /* return k‑th term if it is one of the known initial terms */
    if (n < 7)
        return init[n];

    /* roll‑over buffer for the last seven values */
    seq_t a0 = init[0], a1 = init[1], a2 = init[2], a3 = init[3],
          a4 = init[4], a5 = init[5], a6 = init[6];
    seq_t next;

    /* iterate from 7 up to n, using the recurrence */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) - a(i-2) + a(i-4) - 2*a(i-5) + a(i-6)   */
        next = a6 + a6           /* 2*a6 */
               - a5
               + a3
               - (a2 + a2)     /* 2*a2 */
               + a1;

        /* shift the buffer for the next step */
        a0 = a1; a1 = a2; a2 = a3;
        a3 = a4; a4 = a5; a5 = a6; a6 = next;
    }

    return a6;
}
