/*
 * A250857  Number of (5+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 19, u64 n <= 176, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250857: Number of (5+1) × (n+1) 0..3 arrays with
 * nondecreasing differences in the i direction and
 * nondecreasing min(x(i,j),x(i-1,j)) in the j direction.
 *
 * Recurrence (for n > 7):
 *   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3)
 *          - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7)
 *
 * Initial values:
 *   a(1) = 54410
 *   a(2) = 291165
 *   a(3) = 1043885
 *   a(4) = 2955136
 *   a(5) = 7134786
 *   a(6) = 15344785
 *   a(7) = 30214465
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the initial segment directly */
    static const seq_t init[7] = {
        54410,     /* a(1) */
        291165,    /* a(2) */
        1043885,   /* a(3) */
        2955136,   /* a(4) */
        7134786,   /* a(5) */
        15344785,  /* a(6) */
        30214465   /* a(7) */
    };

    if (n >= 1 && n <= 7)
        return init[n - 1];

    /* Buffer holding the last seven values:
     * buf[0] = a(n-6), ..., buf[6] = a(n)   */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t k = 8; k <= n; ++k)
    {
        seq_t pos = buf[6] * 7 + buf[4] * 35 + buf[2] * 21 + buf[0];
        seq_t neg = buf[5] * 21 + buf[3] * 35 + buf[1] * 7;
        seq_t val = pos - neg;

        /* Rotate buffer left by one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = val;
    }

    return buf[6];
}
