/*
 * A293401  Least integer k such that k/n^2 > (1 + sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A293401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((1+sqrt(5))/2 * n^2)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 100, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition from the OEIS entry */
    if (n == 0)
        return 0;

    /* k = n^2 */
    seq_t k = (seq_t)n * (seq_t)n;

    /* m = 5 * k * k = 5 * n^4 */
    seq_t m = k * k;
    m = m * (seq_t)5;

    /* s = floor(sqrt(m)) = floor(sqrt(5 * n^4)) */
    seq_t s = floor_sqrt(m);

    /* a(n) = floor((k + s) / 2) + 1, which equals ceil(phi * k) */
    seq_t half = (k + s) / (seq_t)2;
    return half + (seq_t)1;
}
