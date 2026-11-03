/*
 * A344510  a(n) = Sum_{k=1..n} k * gcd(k,n).
 * https://oeis.org/A344510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * gcd(k,n)
 * Range:     u8 n <= 11, u16 n <= 139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8540 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t g   = gcd((seq_t)k, N);
        sum += (seq_t)k * g;
    }
    return sum;
}
