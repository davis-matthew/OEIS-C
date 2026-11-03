/*
 * A343005  a(n) is the number of dihedral symmetries D_{2m} (m >= 3) that
 *          configurations of n non-overlapping equal circles can possess.
 * https://oeis.org/A343005
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + tau(n-1) - 3, for n ≥ 2 * where tau(k) is the
 *            number of positive divisors of k
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 85 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n < 2u)
        return 0;
    seq_t t1 = num_divisors(n);
    seq_t t2 = num_divisors(n - 1u);
    return t1 + t2 - 3;
}
