/*
 * A294926  Number of proper divisors of n that are deficient (A005100).
 * https://oeis.org/A294926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d | n, d < n, sigma(d) < 2*d } (number of proper
 *            divisors of n that are deficient)
 * Range:     u8 n <= 9924, u16 n <= 9924, u32 n <= 9924, u64 n <= 9924,
 *            u128 n <= 9924, big any n
 * Helpers:   is_deficient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_deficient.h"

/* Compute floor(sqrt(n)) for uint64_t without overflow. */
static inline idx_t uint64_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;              /* 1 has no proper divisors */

    seq_t count = 0;
    idx_t root = uint64_sqrt(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d != 0)
            continue;
        idx_t d1 = d;
        idx_t d2 = n / d;

        if (d1 < n && is_deficient(d1))
            ++count;

        if (d2 != d1 && d2 < n && is_deficient(d2))
            ++count;
    }
    return count;
}
