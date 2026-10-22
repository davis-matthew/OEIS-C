/*
 * A342534  a(n) = Sum_{k=1..n} phi(gcd(k, n))^2.
 * https://oeis.org/A342534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n} phi(gcd(k,n))^2 Equivalent: a(n) = Sum_{d|n}
 *            phi(n/d) * phi(d)^2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Safe guard – sequence is defined from n=1 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute gcd(k,n) using the helper gcd(a,b) which works on seq_t. */
        seq_t g = gcd((seq_t)k, (seq_t)n);
        /* Convert the gcd back to idx_t for phi. */
        seq_t pg = phi((idx_t)g);
        sum += pg * pg;            /* φ(gcd)^2 */
    }
    return sum;
}
