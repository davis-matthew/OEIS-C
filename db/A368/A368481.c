/*
 * A368481  The degree of polynomials related to Somos-5 sequences. Also for n
 *          > 4 the degree of the (n-4)-th involution in a family of
 *          involutions in the Cremona group of rank 4 defined by a Somos-5
 *          sequence.
 * https://oeis.org/A368481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Style: A linear recurrence derived from the rational generating function.
   Recurrence (valid for n ≥ 10): 
   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-7) - a(n-8) - a(n-9) + a(n-10),
   with initial values
   a(0) = a(1) = a(2) = a(3) = a(4) = 0,
   a(5) = 2, a(6) = 3, a(7) = 4, a(8) = 6, a(9) = 9, a(10) = 11. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the known initial values directly. */
    if (n <= 10) {
        static const seq_t base[11] = { 0, 0, 0, 0, 0, 2, 3, 4, 6, 9, 11 };
        return base[(idx_t)n];
    }

    /* Sliding window of the last 11 values of the sequence.
       buf[0] = a(i-1), buf[1] = a(i-2), …, buf[10] = a(i-11). */
    seq_t buf[11];
    buf[0] = 11;   /* a(10) */
    buf[1] = 9;    /* a(9)  */
    buf[2] = 6;    /* a(8)  */
    buf[3] = 4;    /* a(7)  */
    buf[4] = 3;    /* a(6)  */
    buf[5] = 2;    /* a(5)  */
    buf[6] = 0;    /* a(4)  */
    buf[7] = 0;    /* a(3)  */
    buf[8] = 0;    /* a(2)  */
    buf[9] = 0;    /* a(1)  */
    buf[10] = 0;   /* a(0)  */

    for (idx_t i = 11; i <= n; ++i) {
        seq_t next = buf[0] + buf[1] - buf[2] + buf[6] - buf[7] - buf[8] + buf[9];
        /* Shift the window: buf[k] becomes buf[k-1]. */
        for (int k = 10; k > 0; --k) buf[k] = buf[k-1];
        buf[0] = next;
    }
    return buf[0];
}
