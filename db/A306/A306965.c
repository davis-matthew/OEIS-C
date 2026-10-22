/*
 * A306965  If the decimal expansion of n is d_1 ... d_k, a(n) = Sum
 *          binomial(10,d_i).
 * https://oeis.org/A306965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{digits d of n} binomial(10,d)
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: add the binomial coefficient for each decimal digit of n. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Precomputed binomial(10, d) for d = 0..9 */
    static const seq_t bin10[10] = {1, 10, 45, 120, 210, 252, 210, 120, 45, 10};

    seq_t total = 0;
    if (n == 0) {
        /* Special case: decimal expansion of 0 is a single digit 0 */
        return bin10[0];
    }

    idx_t temp = n;
    while (temp > 0) {
        unsigned d = temp % 10;
        total += bin10[d];
        temp /= 10;
    }
    return total;
}
