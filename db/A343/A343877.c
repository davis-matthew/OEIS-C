/*
 * A343877  Number of pairs (d1, d2) of divisors of n such that d1<d2, d1|n,
 *          d2|n, and d1 + d2 <= n.
 * https://oeis.org/A343877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*tau(n)^2 - (3/2)*tau(n) + 1 * where tau(n) is the
 *            number of divisors of n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the divisor count tau(n) by trial division. */
    uint64_t m = (uint64_t)n;
    uint64_t tau = 1;

    for (uint64_t i = 2; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            uint64_t e = 0;
            while (m % i == 0)
            {
                ++e;
                m /= i;
            }
            tau *= (e + 1);
        }
    }
    if (m > 1)
        tau *= 2;   /* remaining prime factor */

    /* Compute the formula using integer arithmetic in seq_t. */
    seq_t t1 = (seq_t)tau * (seq_t)tau;          /* tau^2   */
    seq_t t2 = (seq_t)3 * (seq_t)tau;            /* 3*tau   */
    seq_t res = t1 / 2;                          /* (tau^2)/2 */
    res -= t2 / 2;                               /* - (3*tau)/2 */
    res += 1;                                    /* + 1 */

    return res;
}
