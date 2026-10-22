/*
 * A363820  Moving the rightmost digit of a number to place it furthest to the
 *          left adds 9 to the number.
 * https://oeis.org/A363820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = repdigit(d,L) - 10 * For n ≤ 8: L = 2, d = n + 1 (digits
 *            2 … 9) * For n > 8: let m = n - 8 * L = 3 + (m - 1) / 9 //
 *            length of the repdigit * d = 1 + (m - 1) % 9 // digit 1 … 9
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 74, u64 n <= 162,
 *            u128 n <= 335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t d;
    idx_t L;
    idx_t m, group, offset;

    if (n <= 8) {
        d = (seq_t)(n + 1);     /* digits 2 … 9 */
        L = 2;
    } else {
        m = n - 8;
        group  = (m - 1) / 9;   /* 0‑based group index */
        L = 3 + group;
        offset = (m - 1) % 9;
        d = (seq_t)(offset + 1); /* digits 1 … 9 */
    }

    seq_t rep = 0;
    for (idx_t i = 0; i < L; ++i)
        rep = rep * 10 + d;     /* build the repdigit */

    return rep - 10;
}
