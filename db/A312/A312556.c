/*
 * A312556  Coordination sequence Gal.6.246.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 with initial values
 *            a(0) = 1, a(1) = 4, a(2) = 8, a(3) = 14, a(4) = 18, a(5) = 24,
 *            a(6) = 28, a(7) = 34, a(8) = 38, a(9) = 44, a(10) = 48, a(11) =
 *            52, a(12) = 56
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* the first 13 terms, used for n <= 12 */
    static const seq_t init[13] = {
        1, 4, 8, 14, 18, 24, 28, 34, 38, 44, 48, 52, 56
    };

    if (n < 13)               /* offset 0 */
        return init[(size_t)n];

    /* buffer that keeps the last 13 computed terms.
       buf[0] = a(i-1), buf[1] = a(i-2), … , buf[12] = a(i-13)      */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i)
        buf[i] = init[12 - i]; /* reverse order: buf[0] = a(12), …, buf[12] = a(0) */

    /* compute from i = 13 up to n */
    for (idx_t i = 13; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-11) - a(i-12) */
        seq_t next = buf[0] + buf[10] - buf[11];

        /* shift buffer right by one position, insert new term at buf[0] */
        for (size_t j = 12; j > 0; --j)
            buf[j] = buf[j - 1];
        buf[0] = next;
    }

    return buf[0]; /* now buf[0] == a(n) */
}
