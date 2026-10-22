/*
 * A337683  Number of distinct positive integer pairs, (s,t), with s < t < n
 *          such that neither s nor t divides n and the harmonic mean of s and
 *          t is an integer.
 * https://oeis.org/A337683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of distinct positive integer pairs (s,t) with * 1 ≤ s <
 *            t < n, s ∤ n, t ∤ n, and 2st/(s+t) is an integer
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   harmonic_integral
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/harmonic_integral.h"

seq_t a(idx_t n)
{
    if (n <= 2u)          /* no pair exists for n = 1,2 */
        return (seq_t)0;

    seq_t count = 0;

    /* Loop over all s < t < n with the restriction that
     * neither s nor t divides n. */
    for (idx_t s = 1; s < n - 1; ++s) {
        if (n % s == 0)          /* s divides n – skip all pairs with this s */
            continue;

        for (idx_t t = s + 1; t < n; ++t) {
            if (n % t == 0)      /* t divides n – skip this pair */
                continue;

            if (harmonic_integral(s, t))
                ++count;
        }
    }
    return count;
}
