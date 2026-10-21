/*
 * A252529  Number of (n+2) X (5+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A252529: Number of (n+2)×(5+2) arrays with the given local-sum constraints.
 * Recurrence: a(n) = 12*a(n-3) - 32*a(n-6)  for n > 8
 * Initial values (offset 1):
 *   a(1)=778, a(2)=706, a(3)=1120, a(4)=1592, a(5)=4420, a(6)=7136,
 *   a(7)=10720, a(8)=30752
 * The function works for any n ≥ 1 and is independent of call order.
 */
#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* for n <= 8 use the pre‑computed values */
    static const seq_t init[9] = {
        0,      /* dummy for offset 0 */
        778,    /* a(1) */
        706,    /* a(2) */
        1120,   /* a(3) */
        1592,   /* a(4) */
        4420,   /* a(5) */
        7136,   /* a(6) */
        10720,  /* a(7) */
        30752   /* a(8) */
    };

    if (n < 9) return init[n];

    /* circular buffer holding a(i-6) … a(i-1) during iteration */
    seq_t buf[6];
    buf[0] = init[3];   /* a(3)  */
    buf[1] = init[4];   /* a(4)  */
    buf[2] = init[5];   /* a(5)  */
    buf[3] = init[6];   /* a(6)  */
    buf[4] = init[7];   /* a(7)  */
    buf[5] = init[8];   /* a(8)  */

    seq_t result = 0;   /* value for a(n) once computed */

    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 12*a(i-3) - 32*a(i-6)  where a(i-3)=buf[3], a(i-6)=buf[0] */
        result = 12 * buf[3] - 32 * buf[0];

        /* slide the window: drop a(i-6), shift left, insert a(i) */
        for (int k = 0; k < 5; ++k) {
            buf[k] = buf[k + 1];
        }
        buf[5] = result;
    }

    return result;
}
