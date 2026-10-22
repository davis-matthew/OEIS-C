/*
 * A362316  Expansion of e.g.f (exp(x)-1)*(exp(2*x)-1).
 * https://oeis.org/A362316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n - 1 for n>0; a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t p3 = pow_uint(3ULL, n);
    seq_t p2 = pow_uint(2ULL, n);
    return p3 - p2 - (seq_t)1;
}
