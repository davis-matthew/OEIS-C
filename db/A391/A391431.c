/*
 * A391431  Numerator of k + a/k where n = k^2 + 2*a + 1 and -k <= a <= k.
 * https://oeis.org/A391431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction (k + a/k), where n =
 *            k^2 + 2*a + 1 and -k <= a <= k
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Search k such that n lies in [(k-1)^2 , (k+1)^2] and (n - k^2 - 1) is even.
       For such k compute a = (n - k^2 - 1)/2 and verify |a| <= k.
       The value a(n) is the numerator of the fraction k + a/k after
       reducing to lowest terms. */
    for (idx_t k = 1; ; ++k) {
        /* (k-1)^2 grows, stop when it's already larger than n. */
        __int128 kk   = (__int128)k * k;
        __int128 min_n = (__int128)(k-1) * (k-1);
        if (min_n > (__int128)n)
            break;

        /* n must also be within the upper bound (k+1)^2. */
        __int128 max_n = (__int128)(k+1) * (k+1);
        if ((__int128)n > max_n)
            continue;

        __int128 diff = (__int128)n - kk - 1;   /* n - k^2 - 1 might be negative */
        if ((diff & 1) != 0)                     /* must be even to give integer a */
            continue;

        __int128 a128 = diff >> 1;               /* a = (n - k^2 - 1)/2 */

        if (a128 < -(__int128)k || a128 > (__int128)k)
            continue;                            /* a out of range */

        /* Build the fraction (k^2 + a)/k and reduce. */
        __int128 numer128 = kk + a128;           /* k^2 + a >= 0 */
        seq_t numer = (seq_t)numer128;
        seq_t denom = (seq_t)k;
        seq_t g = gcd(numer, denom);
        return numer / g;
    }

    /* If no k found (shouldn't happen for n >= 1) return 0 as safe fallback. */
    return (seq_t)0;
}
