/*
 * A330501  Smallest prime of the form m(m+1)/2 - n(n-3)/2 with m >= n, or 0
 *          if no such prime exists.
 * https://oeis.org/A330501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime of the form m(m+1)/2 - n(n-3)/2, with m >=
 *            n; a(n) = 0 if no such prime exists (only n = 3)
 * Range:     u8 n <= 17, u16 n <= 632, u32 n <= 2417, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n = 3 never produces a prime: the expression reduces to a triangular
       number > 3 which is always composite.  Avoid an infinite loop. */
    if (n == 3)
        return 0;

    for (idx_t m = n;; ++m)
    {
        /* Use 128‑bit arithmetic for the intermediate to avoid overflow when
           seq_t is small (up to uint8_t).  The final value fits in seq_t by
           problem definition. */
        __uint128_t mprod = (__uint128_t)m * ((__uint128_t)m + 1);
        __uint128_t nprod = (__uint128_t)n * ((__uint128_t)n - 3);
        __uint128_t num   = mprod - nprod;                /* numerator */
        seq_t p = (seq_t)(num / 2);                       /* candidate prime */

        if (is_prime_big(p))
            return p;
    }
}
