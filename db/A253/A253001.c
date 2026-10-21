/*
 * A253001  Number of n X 5 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 3 and every value
 *          within 3 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 52, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n > 12
   Initial values: a(1)=4, a(2)=14, a(3)=34, a(4)=69, a(5)=69,
   a(6)=3072, a(7)=39758, a(8)=228484, a(9)=775433, a(10)=1932763,
   a(11)=3965261, a(12)=7139167 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for 1 <= n <= 12 */
    static const seq_t base[12] = {
        (seq_t)4,      (seq_t)14,      (seq_t)34,     (seq_t)69,
        (seq_t)69,     (seq_t)3072,    (seq_t)39758,  (seq_t)228484,
        (seq_t)775433, (seq_t)1932763, (seq_t)3965261,(seq_t)7139167
    };

    if (n == 0) {
        /* Undefined for n == 0 under OEIS offset, return 0 as guard */
        return (seq_t)0;
    }

    if (n <= 12) {
        return base[n-1];
    }

    /* Start recurrence from n = 13 using a(9) .. a(12) */
    seq_t a1 = base[8];   /* a(n-4) at n=13 */
    seq_t a2 = base[9];   /* a(n-3) */
    seq_t a3 = base[10];  /* a(n-2) */
    seq_t a4 = base[11];  /* a(n-1) */

    for (idx_t i = 13; i <= n; ++i) {
        /* a(i) = 4*a4 - 6*a3 + 4*a2 - a1 */
        seq_t next = 4*a4 - 6*a3 + 4*a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
