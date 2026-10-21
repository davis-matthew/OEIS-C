/*
 * A254333  Squares (A000290) which are also centered pentagonal numbers
 *          (A005891).
 * https://oeis.org/A254333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1442*a(n-2)-1442*a(n-3)-a(n-4)+a(n-5); initial
 *            values: a(1)=1, a(2)=16, a(3)=1156, a(4)=22801, a(5)=1666681
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)1156;
    if (n == 4) return (seq_t)22801;
    if (n == 5) return (seq_t)1666681;

    /* sliding window for the last five values */
    seq_t v[5] = { (seq_t)1,
                   (seq_t)16,
                   (seq_t)1156,
                   (seq_t)22801,
                   (seq_t)1666681 };

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t newv = v[4] + (seq_t)1442 * v[3]
                   - (seq_t)1442 * v[2]
                   - v[1] + v[0];

        /* shift window */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = newv;
    }

    return v[4];
}
