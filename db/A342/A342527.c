/*
 * A342527  Number of compositions of n with alternating parts equal.
 * https://oeis.org/A342527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n + σ(n) - 2·d(n) (σ(n) = sum of divisors of n, d(n)
 *            = number of divisors of n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t (used by idx_t) */

seq_t a(idx_t n)
{
    /* Special case for the offset 0 */
    if (n == 0)
        return 1;

    /* compute number of divisors d and sum of divisors σ */
    seq_t sigma = 0;
    seq_t d = 0;

    /* iterate over divisors using i up to sqrt(n) without overflow */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            seq_t j = (seq_t) n / i;
            sigma += (seq_t) i;
            ++d;
            if (j != i) /* avoid double count for perfect squares */
            {
                sigma += j;
                ++d;
            }
        }
    }

    /* a(n) = 1 + n + σ(n) - 2·d(n) */
    seq_t result = 1 + (seq_t) n + sigma - 2 * d;
    return result;
}
