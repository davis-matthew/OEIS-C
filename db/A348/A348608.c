/*
 * A348608  a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(d + n/d) * d.
 * https://oeis.org/A348608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(d + n/d) * d
 * Range:     i8 n <= 989, i16 n <= 7285, i32 n <= 7285, i64 n <= 7285,
 *            i128 n <= 7285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for bool usage (optional) */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* Iterate over divisor candidates d <= sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;          /* d is not a divisor */
        idx_t m = n / d;                    /* paired divisor */
        /* Determine parity of d + m using bits to avoid overflow */
        int parity = ((d & 1) ^ (m & 1));   /* 1 if odd, 0 if even */
        seq_t term = (seq_t)d;              /* convert to seq_t for addition */
        if (parity) term = -term;           /* (-1)^(d+m) */
        res += term;
    }
    return res;
}
