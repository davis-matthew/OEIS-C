/*
 * A397935  Number of n X n binary matrices with exactly 14 1's such that
 *          there are an even number of 1's in every row and column.
 * https://oeis.org/A397935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3110940*C(n,7)^2 + 4233600*C(n,7)*C(n,6) +
 *            911400*C(n,7)*C(n,5) * + 65520*C(n,7)*C(n,4) +
 *            1092*C(n,7)*C(n,3) + 1080000*C(n,6)^2 * + 316800*C(n,6)*C(n,5) +
 *            11520*C(n,6)*C(n,4) + 14600*C(n,5)^2 * + 480*C(n,5)*C(n,4) *
 *            where C(n,k) is the binomial coefficient. * This expression is
 *            valid for a...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 29, u128 n <= 500,
 *            big any n
 * Helpers:   binomial_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_simple.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    seq_t c7 = binomial_simple(n, 7);
    seq_t c6 = binomial_simple(n, 6);
    seq_t c5 = binomial_simple(n, 5);
    seq_t c4 = binomial_simple(n, 4);
    seq_t c3 = binomial_simple(n, 3);

    seq_t sum = 0;
    sum += 3110940ULL * c7 * c7;
    sum += 4233600ULL * c7 * c6;
    sum += 911400ULL  * c7 * c5;
    sum += 65520ULL   * c7 * c4;
    sum += 1092ULL    * c7 * c3;
    sum += 1080000ULL * c6 * c6;
    sum += 316800ULL  * c6 * c5;
    sum += 11520ULL   * c6 * c4;
    sum += 14600ULL   * c5 * c5;
    sum += 480ULL     * c5 * c4;
    return sum;
}
