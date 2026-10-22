/*
 * A385911  If n = 2^b*3^c, then a(n) = (-1)^b * binomial(b+c, b), else a(n) =
 *          0, for n >= 1.
 * https://oeis.org/A385911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^b * binomial(b+c, b) if n = 2^b * 3^c, else 0
 *            (offset 1)
 * Range:     i8 n <= 97, i16 n <= 97, i32 n <= 97, i64 n <= 97, i128 n <= 97,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    size_t b = 0;
    size_t c = 0;
    idx_t tmp = n;

    /* extract powers of 2 */
    while ((tmp & 1ULL) == 0 && tmp > 0) {
        tmp >>= 1;
        ++b;
    }

    /* extract powers of 3 */
    while (tmp % 3ULL == 0 && tmp > 0) {
        tmp /= 3;
        ++c;
    }

    /* if any other prime factor remains, a(n)=0 */
    if (tmp != 1)
        return 0;

    /* compute binomial(b+c, b) */
    seq_t bin = binomial((idx_t)(b + c), (idx_t)b);

    /* apply sign (-1)^b */
    seq_t sign = (b & 1ULL) ? -1 : 1;

    return sign * bin;
}
