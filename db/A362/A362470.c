/*
 * A362470  Number of divisors d of n such that phi(d) | n.
 * https://oeis.org/A362470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ d | n : phi(d) | n }
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t phi_d = phi(d);
            if ((seq_t)n % phi_d == 0)
                ++count;
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t phi_d2 = phi(d2);
                if ((seq_t)n % phi_d2 == 0)
                    ++count;
            }
        }
    }
    return count;
}
