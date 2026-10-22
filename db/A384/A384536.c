/*
 * A384536  a(n) = 4^n - 2^(n-6)*15*binomial(n,6).
 * https://oeis.org/A384536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^(n-6)*15*binomial(n,6)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute 4^n as 2^(2*n) */
    idx_t pow2exp = (idx_t)2 * n;
    seq_t term = pow2(pow2exp);

    /* Subtraction term is zero for n < 6 */
    if (n < 6u) {
        return term;
    }

    /* Compute 2^(n-6) * 15 * binomial(n,6) */
    seq_t binom  = binomial(n, 6u);
    seq_t pow2part = pow2(n - 6u);

    seq_t sub = pow2part * (seq_t)15;
    sub = sub * binom;

    return term - sub;
}
