/*
 * A280579  Square array read by antidiagonals downwards giving the first
 *          differences A261327(n+p) - A261327(n), with p >= 0.
 * https://oeis.org/A280579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,p) = A261327(n+p) - A261327(n), read by antidiagonals; a(t)
 *            with d = floor((sqrt(8t+1)-1)/2), i = t - d(d+1)/2: a(t) =
 *            A261327(d) - A261327(d-i). A261327(2k) = 1 + k^2, A261327(2k+1)
 *            = 5 + 4k(k+1)
 * Range:     i8 n <= 58, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t A261327_a(idx_t n)
{
    seq_t k = (seq_t)(n >> 1);
    if ((n & 1) == 0) {
        return 1 + k * k;
    } else {
        return 5 + 4 * k * (k + 1);
    }
}

seq_t a(idx_t n)
{
    idx_t d = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while (d > 0 && d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    idx_t i = n - d * (d + 1) / 2;   /* p = i, column m = d - i */
    idx_t m = d - i;
    return A261327_a(d) - A261327_a(m);
}
