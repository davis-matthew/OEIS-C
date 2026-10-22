/*
 * A318459  a(n) = gcd(n, tau(n), phi(n)), where tau = A000005 and phi =
 *          A000010.
 * https://oeis.org/A318459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, tau(n), phi(n))
 * Range:     u8 n <= 1365, u16 n <= 8722, u32 n <= 8722, u64 n <= 8722,
 *            u128 n <= 8722, big any n
 * Helpers:   divisor_count_u64, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t nn = n;                /* n as seq_t */
    seq_t tau = divisor_count_u64((uint64_t)n);
    seq_t ph = phi(n);
    seq_t g = gcd(nn, tau);
    g = gcd(g, ph);
    return g;
}
