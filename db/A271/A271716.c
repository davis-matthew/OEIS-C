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
 * Helpers:   divisor_count_u64, highest_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/highest_bit.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* sequence offset starts at 1 */

    seq_t sum = 0;
    idx_t hb = highest_bit(n);

    for (idx_t i = 0; i <= hb; ++i) {
        uint64_t m = ((uint64_t)n) ^ ((uint64_t)1 << i);
        if (m == 0)                  /* m must be positive */
            continue;
        seq_t d = divisor_count_u64(m);
        sum += d;
    }
    return sum;
}
