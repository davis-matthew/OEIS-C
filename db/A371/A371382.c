/*
 * A371382  a(n) = n^2 + q*(q + 1), where q = floor(n*(1 + sqrt(5))/2) =
 *          A000201(n).
 * https://oeis.org/A371382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + q*(q+1), where q = floor(n*phi) and phi = (1 +
 *            sqrt(5))/2. We compute q as (n + floor(n*sqrt(5)))/2 using
 *            integer arithmetic
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    /* use seq_t for all intermediate computations */
    seq_t nn = (seq_t)n;

    /* floor(n * sqrt(5)) = floor(sqrt(5 * n^2)) */
    seq_t t = floor_sqrt((seq_t)5 * nn * nn);

    /* q = floor((n + floor(n*sqrt(5)))/2) */
    seq_t q = (nn + t) / 2;

    /* a(n) = n^2 + q*(q+1) */
    return nn * nn + q * (q + 1);
}
