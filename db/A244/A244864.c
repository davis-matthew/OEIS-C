/*
 * A244864  a(n) = binomial(n+5,5) + 4*binomial(n+4,5) + 4*binomial(n+3,5) +
 *          binomial(n+2,5).
 * https://oeis.org/A244864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) + 4*binomial(n+4,5) + 4*binomial(n+3,5) +
 *            binomial(n+2,5)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 947 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the four binomial terms.  binom handles integer
       conversion and uses only integer arithmetic internally. */
    seq_t t1 = binomial(n + 5, 5);  /* C(n+5,5) */
    seq_t t2 = binomial(n + 4, 5);  /* C(n+4,5) */
    seq_t t3 = binomial(n + 3, 5);  /* C(n+3,5) */
    seq_t t4 = binomial(n + 2, 5);  /* C(n+2,5) */

    /* Combine with the given coefficients. */
    return t1 + 4 * t2 + 4 * t3 + t4;
}
