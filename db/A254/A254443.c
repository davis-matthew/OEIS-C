/*
 * A254443  Numbers n such that T(n) + T(n+1) + ... + T(n+21) is a square,
 *          where T(m) = A000217(m) is the m-th triangular number.
 * https://oeis.org/A254443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254443
   a(1) = 35, a(2) = 75, a(3) = 911, a(4) = 1707,
   for n >= 5  a(n) = 20*a(n-2) - a(n-4) + 198
   (coefficients derived from the rational g.f.).
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values a(1) .. a(4) */
    static const seq_t init[5] = {0, 35, 75, 911, 1707};

    if (n <= 4) return init[n];

    /* circular buffer for the last four values, indexed
       by (index-1) % 4.  At start it contains a(1) .. a(4). */
    seq_t last[4];
    last[0] = init[1];  /* a(1)  */
    last[1] = init[2];  /* a(2)  */
    last[2] = init[3];  /* a(3)  */
    last[3] = init[4];  /* a(4)  */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i-2)  -> last[(i-3) % 4]  (since i-2-1 = i-3)
           a(i-4)  -> last[(i-5) % 4]  (since i-4-1 = i-5)   */
        seq_t term2 = last[(i - 3) % 4];
        seq_t term4 = last[(i - 5) % 4];
        seq_t next  = (seq_t)20 * term2 - term4 + (seq_t)198;
        last[(i - 1) % 4] = next;
    }

    return last[(n - 1) % 4];
}
