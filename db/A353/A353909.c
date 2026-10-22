/*
 * A353909  a(n) is the alternating sum of the sequence gcd(n, k^2), 1 <= k <=
 *          n.
 * https://oeis.org/A353909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1..n} (-1)^i * gcd(n, i^2)
 * Range:     i8 n <= 11, i16 n <= 208, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ig2 = (seq_t)i * (seq_t)i;          /* i^2 as seq_t */
        seq_t g = gcd((seq_t)n, ig2);            /* gcd(n, i^2) */
        if (i & 1)  /* odd i: sign = -1 */
            sum -= g;
        else        /* even i: sign = +1 */
            sum += g;
    }
    return sum;
}
