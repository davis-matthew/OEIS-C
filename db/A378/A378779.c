/*
 * A378779  a(n) = n^2 * 4^n * binomial(3*n, n).
 * https://oeis.org/A378779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * 4^n * binomial(3*n, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    idx_t two_n = n << 1;          /* 2*n */
    seq_t result = 1;

    /* Compute binomial(3n, n) as product_{i=1..n} (2n + i) / i */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (seq_t)(two_n + i);
        result = result / (seq_t)i;
    }

    /* 4^n = 2^{2n} */
    seq_t four_power = pow2(two_n);

    /* n^2 */
    seq_t nn   = (seq_t)n;
    seq_t n_sq = nn * nn;

    seq_t val = result * n_sq;
    val = val * four_power;
    return val;
}
