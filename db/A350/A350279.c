/*
 * A350279  Irregular triangle T(n,k) read by rows in which row n lists the
 *          iterates of the Farkas map (A349407) from 2*n - 1 to 1.
 * https://oeis.org/A350279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   T(n,1) = 2*n-1; T(n,k) = A349407((T(n,k-1)+1)/2); row n ends at
 *            1; a(m) is read off the triangle by rows
 * Range:     u8 n <= 261, u16 n <= 12301, u32 n <= 12301, u64 n <= 12301,
 *            u128 n <= 12301, big any n
 * Validated: standard, pass, 88 terms through n = 9137 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A350279_step(seq_t x)
{
    /* Farkas map applied to odd x (x = 2*index-1) */
    if (x % 3 == 0)
        return x / 3;
    else if (x % 4 == 3)
        return (3 * x + 1) / 2;
    else
        return (x + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t start = 1;
    for (;;) {
        idx_t len = 1;
        seq_t v = start;
        while (v != 1) {
            v = A350279_step(v);
            len++;
        }
        if (m <= len) {
            v = start;
            idx_t i;
            for (i = 1; i < m; i++)
                v = A350279_step(v);
            return v;
        }
        m -= len;
        start += 2;
    }
}
