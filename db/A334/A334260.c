/*
 * A334260  Sum of the largest composite parts in the partitions of 2n into
 *          two parts.
 * https://oeis.org/A334260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=n}^{2n-1} m * (1 - is_prime_small(m)) with m≠1 = sum
 *            of the largest composite parts in the two‑part partitions of 2n
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* Include the prime test utility */
#include "../utils/is_prime_small.h"


seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n=0, return 0 harmlessly */
        return 0;

    idx_t start = n;
    idx_t end   = 2 * n - 1;   /* 2n‑1 is safe for uint64_t when n ≤ 10⁴ */

    seq_t sum = 0;
    for (idx_t m = start; m <= end; ++m) {
        if (m == 1)             /* exclude the unit part */
            continue;
        if (!is_prime_small((seq_t)m))
            sum += (seq_t)m;
    }
    return sum;
}
