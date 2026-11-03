/*
 * A318459  a(n) = gcd(n, tau(n), phi(n)), where tau = A000005 and phi =
 *          A000010.
 * https://oeis.org/A318459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, tau(n), phi(n))
 * Range:     u8 n <= 1365, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8722 (2026-11-01)
 * Helpers:   gcd, num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t nn = n;                /* n as seq_t */
    seq_t tau = num_divisors((uint64_t)n);
    seq_t ph = totient(n);
    seq_t g = gcd(nn, tau);
    g = gcd(g, ph);
    return g;
}
