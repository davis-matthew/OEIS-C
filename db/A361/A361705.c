/*
 * A361705  Constant term in the expansion of (1 + w^4 + x^4 + y^4 + z^4 +
 *          1/(w*x*y*z))^n.
 * https://oeis.org/A361705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/8)} (4k)!/k!^4 * binomial(8k,4k) *
 *            binomial(n,8k)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the identity (4k)!/k!^4 = binomial(4k,k)*binomial(3k,k)*binomial(2k,k) */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 8;            /* floor(n/8) */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = binomial(4 * k, k);
        term = term * binomial(3 * k, k);
        term = term * binomial(2 * k, k);
        term = term * binomial(8 * k, 4 * k);
        term = term * binomial(n, 8 * k);
        sum += term;
    }
    /* For n < 8 the loop runs once with k = 0, giving term = 1. */
    return sum;
}
