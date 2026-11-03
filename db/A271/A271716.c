/*
 * A271716  Sum of numbers of divisors of positive numbers differing from n in
 *          one bit.
 * https://oeis.org/A271716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m differs from n in exactly one binary bit} d(m),
 *            where d(m) = number of positive divisors of m (A000005)
 * Range:     u8 n <= 2215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9130 (2026-11-01)
 * Helpers:   ilog2_idx, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ilog2_idx.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* sequence offset starts at 1 */

    seq_t sum = 0;
    idx_t hb = ilog2_idx(n);

    for (idx_t i = 0; i <= hb; ++i) {
        uint64_t m = ((uint64_t)n) ^ ((uint64_t)1 << i);
        if (m == 0)                  /* m must be positive */
            continue;
        seq_t d = num_divisors(m);
        sum += d;
    }
    return sum;
}
