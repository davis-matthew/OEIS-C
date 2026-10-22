/*
 * A337971  a(n) = 5^(n*(n-1)/2) + 5^(n*(n+1)/2) for n > 0, with a(0) = 1.
 * https://oeis.org/A337971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n*(n-1)/2) + 5^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Exponents for the two powers of 5. */
    idx_t exp1 = n * (n - 1) / 2;
    idx_t exp2 = n * (n + 1) / 2;

    seq_t p1 = power5(exp1);
    seq_t p2 = power5(exp2);

    return p1 + p2;
}
