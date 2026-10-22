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
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

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
