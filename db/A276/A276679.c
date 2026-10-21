/*
 * A276679  Number of divisors of the n-th hexagonal number.
 * https://oeis.org/A276679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * d(2n-1) where d(k) is the number of divisors of k.
 *            The nth hexagonal number is H_n = n(2n-1). Since gcd(n,2n-1)=1,
 *            divisor_count(H_n) = divisor_count(n) * divisor_count(2n-1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* n is 1-based; result for n==0 is undefined, but we return 0 to
       keep the function well‑formed. */
    if (n == 0) return 0ULL;

    uint64_t nn   = (uint64_t)n;
    uint64_t part1 = nn;          /* n */
    uint64_t part2 = (nn << 1) - 1ULL;  /* 2n-1 */

    seq_t cnt1 = divisor_count_u64(part1);
    seq_t cnt2 = divisor_count_u64(part2);

    return cnt1 * cnt2;
}
