/*
 * A376551  This sequence satisfies: n = Sum_{k=0..n} ((-n)^(n -
 *          k)*binomial(n, k)*a(k)).
 * https://oeis.org/A376551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{k=0..n} ( binomial(n, k) * (n - k)^k )
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= n; ++k) {
        /* binomial coefficient C(n, k) */
        seq_t C = binomial(n, k);

        /* (n - k)^k  (base is uint64_t, exponent is idx_t) */
        seq_t power = pow_uint((uint64_t)(n - k), k);

        /* term = C * power */
        sum += C * power;
    }

    /* a(n) = n * sum */
    return (seq_t)n * sum;
}
