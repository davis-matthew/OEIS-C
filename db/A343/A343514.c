/*
 * A343514  a(n) = Sum_{k=1..n} (k/gcd(n, k))^4.
 * https://oeis.org/A343514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..n} (k/gcd(n,k))^4
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 120, u64 n <= 9850,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t g = gcd(kk, sn);
        seq_t t = kk / g;
        seq_t t2 = t * t;
        seq_t t4 = t2 * t2;
        sum += t4;
    }
    return sum;
}
