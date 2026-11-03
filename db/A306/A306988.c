/*
 * A306988  a(n) = Sum_{k=1..n} binomial(n,k)*phi(k), where phi is the Euler
 *          totient function.
 * https://oeis.org/A306988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)*phi(k)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 2618 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t binom = 1;   /* C(n,0) */
    for (idx_t k = 1; k <= n; ++k)
    {
        binom = (binom * (seq_t)(n - k + 1)) / k;   /* C(n,k) */
        sum  += binom * totient(k);
    }
    return sum;
}
