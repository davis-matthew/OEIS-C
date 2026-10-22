/*
 * A356655  Clausen numbers based on the strictly proper divisors of n, 1 < d
 *          < n.
 * https://oeis.org/A356655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n, 1 < d < n} (d + 1) if d + 1 is prime (otherwise
 *            that factor is omitted). The product defaults to 1 when no such
 *            divisors exist
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   _prime_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_prime_uint32.h"

seq_t a(idx_t n)
{
    /* For n = 0,1,2,3 there are no strict proper divisors >1. */
    if (n <= 3U)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* d is a proper divisor >1 < n */
        uint32_t val = (uint32_t)(d + 1);
        if (_prime_uint32(val))
            prod *= (seq_t)val;

        idx_t other = n / d;
        if (other != d) {                 /* avoid double‑counting a square root */
            val = (uint32_t)(other + 1);
            if (_prime_uint32(val))
                prod *= (seq_t)val;
        }
    }
    return prod;
}
