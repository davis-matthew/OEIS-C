/*
 * A274724  Number of nX4 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) or (-1,0) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A274724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+13*a(n-2)+14*a(n-3)+4*a(n-4)-28*a(n-5)-13*a(n-6)-7*a(n-7)+15*a(n-8)
 *            for n>12 * with initial values: * a(1)=14, a(2)=96, a(3)=296,
 *            a(4)=1632, a(5)=9308, * a(6)=52912, a(7)=301784, a(8)=1721528,
 *            a(9)=9816048, * a(10)=55981988, a(11)=319253288,
 *            a(12)=1820657976
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 12 terms */
    static const seq_t init[12] = {
        (seq_t)14,          (seq_t)96,           (seq_t)296,
        (seq_t)1632,        (seq_t)9308,         (seq_t)52912,
        (seq_t)301784,      (seq_t)1721528,      (seq_t)9816048,
        (seq_t)55981988,    (seq_t)319253288,    (seq_t)1820657976
    };

    if (n == 0)
        return (seq_t)0;          /* not used, offset is 1 */

    if (n <= 12)
        return init[n - 1];

    /* buffer holds a(n-8) .. a(n-1) for the current computation */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[4 + i]; /* a(5) .. a(12)  */

    idx_t pos = 0; /* buf[pos] is a(n-8) */

    for (idx_t cur = 13; cur <= n; ++cur) {
        /* indices of required terms */
        seq_t a0 = buf[pos];                 /* a(n-8)   */
        seq_t a1 = buf[(pos + 1) & 7];       /* a(n-7)   */
        seq_t a2 = buf[(pos + 2) & 7];       /* a(n-6)   */
        seq_t a3 = buf[(pos + 3) & 7];       /* a(n-5)   */
        seq_t a4 = buf[(pos + 4) & 7];       /* a(n-4)   */
        seq_t a5 = buf[(pos + 5) & 7];       /* a(n-3)   */
        seq_t a6 = buf[(pos + 6) & 7];       /* a(n-2)   */
        seq_t a7 = buf[(pos + 7) & 7];       /* a(n-1)   */

        seq_t res = 3 * a7 + 13 * a6 + 14 * a5 + 4 * a4 + 15 * a0;
        res -= 28 * a3;
        res -= 13 * a2;
        res -= 7 * a1;

        buf[pos] = res;           /* store new a(n) */
        pos = (pos + 1) & 7;       /* advance ring buffer */
    }

    /* a(n) is stored in the slot before current pos */
    return buf[(pos + 7) & 7];
}
