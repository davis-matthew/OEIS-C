/*
 * A366938  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+2,3) * floor(n/k).
 * https://oeis.org/A366938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+2,3) * floor(n/k)
 * Range:     i8 n <= 10, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   binom_kplus2_choose3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_kplus2_choose3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety, offset starts at 1 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                /* floor(n/k) */
        seq_t bin = binom_kplus2_choose3(k);
        seq_t term = bin * (seq_t)t;
        if (k & 1)                     /* odd k: (+) */
            sum += term;
        else                           /* even k: (-) */
            sum -= term;
    }
    return sum;
}
