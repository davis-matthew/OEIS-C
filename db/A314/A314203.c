/*
 * A314203  Coordination sequence Gal.5.88.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) initial
 *            values: a(0)=1, a(1)=5, a(2)=11, a(3)=17, a(4)=22, a(5)=26,
 *            a(6)=30, a(7)=35, a(8)=41, a(9)=47, a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 11 terms of the sequence */
    static const seq_t init[11] = {
        1, 5, 11, 17, 22, 26, 30, 35, 41, 47, 52
    };

    if (n <= 10)
        return init[(size_t)n];

    /* buffer holding the 10 previous terms:
       buf[0] = a(k-10), buf[9] = a(k-1)   when computing a(k)   */
    seq_t buf[10];
    for (int i = 0; i < 10; ++i)
        buf[i] = init[i + 1];          /* a(1) .. a(10) */

    seq_t res = 0;                     /* will hold a(k) */

    for (idx_t k = 11; k <= n; ++k) {
        /* compute a(k) from the 10 preceding terms in buf */
        res = 0;
        for (int i = 0; i < 9; ++i) {          /* a(k-1) .. a(k-9) */
            seq_t term = 2 * buf[9 - i];       /* coefficient ±2 */
            if (i % 2 == 0)  /* even i : +2 */
                res += term;
            else             /* odd i  : -2 */
                res -= term;
        }
        res -= buf[0];          /* subtract a(k-10) */

        /* shift buffer for next step */
        for (int i = 0; i < 9; ++i)
            buf[i] = buf[i + 1];
        buf[9] = res;
    }

    return res;
}
