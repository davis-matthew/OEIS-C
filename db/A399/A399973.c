/*
 * A399973  a(n) is the sum of sigma (A000203), over the integers smaller or
 *          equal to n, that are not coprime to n.
 * https://oeis.org/A399973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= k <= n; gcd(k,n) > 1} sigma(k)
 * Range:     u8 n <= 21, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9338 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t g = gcd((seq_t)k, (seq_t)n);
        if (g > 1)
        {
            total += sigma_of(k);
        }
    }
    return total;
}
