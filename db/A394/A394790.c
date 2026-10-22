/*
 * A394790  Product of the sum of divisors of n with the exponent of the
 *          highest power of 2 dividing 2n: a(n) = A000203(n)*A001511(n).
 * https://oeis.org/A394790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = A000203(n) * A001511(n)
 * Range:     u8 n <= 31, u16 n <= 2879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = sigma(n) * (exponent of the highest power of 2 dividing 2n) */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);

    /* Count the exponent of 2 dividing n */
    idx_t tmp = n;
    unsigned cnt = 0;
    while ((tmp & 1ULL) == 0) {
        cnt++;
        tmp >>= 1;
    }

    /* Exponent of the highest power of 2 dividing 2n is cnt + 1 */
    seq_t pow2_exp = (seq_t)(cnt + 1);

    return sigma * pow2_exp;
}
