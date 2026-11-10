/*
 * A352183  Erroneous version of A046176.
 * https://oeis.org/A352183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A046176(n) for n != 4, where A046176(n) = 34*A046176(n-1)
 *            - A046176(n-2), A046176(1) = 1, A046176(2) = 35; the erroneous
 *            entry has a typo at n = 4: a(4) = 40931 (digits transposed from
 *            the correct 40391)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 4, i128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t A046176_a(idx_t n)
{
    seq_t p, q, t;
    idx_t i;
    if (n == 0) return 0;
    p = 1;
    if (n == 1) return p;
    q = 35;
    for (i = 3; i <= n; ++i) {
        t = (seq_t)34 * q - p;
        p = q;
        q = t;
    }
    return q;
}

seq_t a(idx_t n)
{
    /* The erroneous version differs from A046176 only by the typo at n = 4. */
    if (n == 4) return (seq_t)40931;
    return A046176_a(n);
}
