/*
 * A326034  Number of knapsack partitions of n with largest part 3.
 * https://oeis.org/A326034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into 1,2,3 with largest part 3 that
 *            are knapsack (every sub-multiset has a distinct sum). Denote by
 *            (a,b,c) the multiplicities of 3, 2, 1 respectively (a≥1). The
 *            partition is knapsack iff: * c = 0 and (a == 1 or b ≤ 2) * c > 0
 *            and b = 0 and c ≤ 2 Therefore a(n) = count...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t max_a = n / 3;              /* floor(n/3) */
    for (idx_t a1 = 1; a1 <= max_a; ++a1) {
        idx_t rem = n - 3 * a1;       /* n - 3a */
        if (rem & 1)                  /* rem odd -> no integer b */
            continue;
        idx_t b = rem >> 1;           /* b = (n - 3a) / 2 */
        if (a1 == 1 || b <= 2)
            ++res;
    }
    if (n >= 4 && n % 3 == 1)
        ++res;                       /* c = 1 case */
    if (n >= 5 && n % 3 == 2)
        ++res;                       /* c = 2 case */
    return res;
}
