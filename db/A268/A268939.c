/*
 * A268939  Number of length-n 0..3 arrays with no repeated value unequal to
 *          the previous repeated value plus one mod 3+1.
 * https://oeis.org/A268939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 12*a(n-3); Initial values: a(1)=4,
 *            a(2)=16, a(3)=60
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, return 0 */
        return (seq_t)0;
    if (n == 1)                /* a(1) = 4 */
        return (seq_t)4;
    if (n == 2)                /* a(2) = 16 */
        return (seq_t)16;
    if (n == 3)                /* a(3) = 60 */
        return (seq_t)60;

    /* Use a rolling window: a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)4;       /* a(1) */
    seq_t a2 = (seq_t)16;      /* a(2) */
    seq_t a3 = (seq_t)60;      /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* next = 5*a3 - 2*a2 - 12*a1; perform in stages to avoid
           unsigned underflow and keep intermediate values small   */
        next = 5 * a3;
        next -= 2 * a2;
        next -= 12 * a1;

        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
