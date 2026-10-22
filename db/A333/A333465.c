/*
 * A333465  a(n) = Sum_{k=1..n} ceiling(n/k) * gcd(n,k).
 * https://oeis.org/A333465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..n} ceil(n/k) * gcd(n,k)
 * Range:     u8 n <= 23, u16 n <= 1079, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t ceil_val = (n + k - 1) / k;           /* ceil(n/k) */
        seq_t g = gcd((seq_t)n, (seq_t)k);          /* gcd(n,k)   */
        sum += ((seq_t)ceil_val) * g;               /* accumulate */
    }
    return sum;
}
