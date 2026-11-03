/*
 * A303781  a(2) = 1; for n <> 2, a(n) = gcd(n, A000005(n)), where A000005(n)
 *          = number of divisors of n.
 * https://oeis.org/A303781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2) = 1; for n <> 2, a(n) = gcd(n, A000005(n))
 * Range:     u8 n <= 291, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8912 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 2)
    {
        return (seq_t)1;
    }
    seq_t n_seq = (seq_t)n;
    seq_t d = (seq_t)num_divisors((uint64_t)n);
    return gcd(n_seq, d);
}
