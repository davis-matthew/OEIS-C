/*
 * A395655  Expansion of 1/(4*x) * (1/(3-2*g)^2 - 1), where g = 1+x*g^3 is the
 *          g.f. of A001764.
 * https://oeis.org/A395655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Σ_{k=0}^{n+1} 2^(n+1-k) * binomial(3*n+2, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* m = 3*n + 2 */
    idx_t m = 3 * n + 2;

    /* current binomial coefficient C(m, k), starting with k = 0 */
    seq_t current = 1;

    /* current power of two 2^(n+1-k), initially k = 0 */
    seq_t pow = pow2(n + 1);

    seq_t sum = 0;

    for (idx_t k = 0; k <= n + 1; ++k)
    {
        /* term = current * pow */
        sum += current * pow;

        /* update binomial coefficient and power for next k */
        if (k < n + 1)
        {
            current = current * (seq_t)(m - k) / (seq_t)(k + 1);
            pow >>= 1;          /* divide by 2 */
        }
    }

    /* divide by 4 */
    return sum / 4;
}
