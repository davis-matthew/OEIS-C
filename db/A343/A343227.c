/*
 * A343227  a(n) = sigma(n) / gcd(sigma(n), n+A003415(n)), where A003415 is
 *          the arithmetic derivative, and sigma is the sum of divisors of n.
 * https://oeis.org/A343227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), n + A003415(n))
 * Range:     u8 n <= 63, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   derivative_of_idx, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/derivative_of_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 but not used */

    seq_t sigma = sigma_of(n);
    seq_t d = derivative_of_idx(n);
    seq_t denom = d + (seq_t)n;
    seq_t g = gcd(sigma, denom);
    return sigma / g;
}
