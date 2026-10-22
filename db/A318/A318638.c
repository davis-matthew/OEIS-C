/*
 * A318638  Expansion of Sum_{n>=1} ( (3 + x^n)^n - 3^n ).
 * https://oeis.org/A318638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 3^(d - n/d) * binomial(d, n/d)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        idx_t e = n / d;

        if (d == e)            /* d == e, exponent is 0 */
        {
            result += pow3(0) * binomial(d, e); /* binomial(d, d) == 1 */
        }
        else if (d < e)        /* divisor e > d, we compute it here */
        {
            idx_t exp = e - d;            /* e - n/e = e - d ≥ 0 */
            seq_t term = pow3(exp) * binomial(e, d);
            result += term;
        }
        /* when d > e the corresponding divisor has already been handled
           as e in a previous iteration, so nothing is needed here. */
    }

    return result;
}
