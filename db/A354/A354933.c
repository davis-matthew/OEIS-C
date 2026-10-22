/*
 * A354933  a(1) = 1; for n > 1, a(n) = n / the largest divisor of n that is
 *          coprime to a larger divisor of n.
 * https://oeis.org/A354933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest divisor d of n with d >= sqrt(n) and gcd(d,
 *            n/d) = 1
 * Range:     u8 n <= 255, u16 n <= 9770, u32 n <= 9770, u64 n <= 9770,
 *            u128 n <= 9770, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* integer sqrt: floor_sqrt of n */
    idx_t s = 1;
    while ((unsigned __int128)(s + 1) * (s + 1) <= (unsigned __int128)n)
        ++s;

    /* ceil of sqrt(n) */
    if ((unsigned __int128)s * s < (unsigned __int128)n)
        ++s;

    for (idx_t d = s; d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t q = n / d;
            if (gcd((seq_t)d, (seq_t)q) == (seq_t)1)
                return (seq_t)d;
        }
    }

    /* Fallback: n is itself the only divisor satisfying the condition. */
    return (seq_t)n;
}
