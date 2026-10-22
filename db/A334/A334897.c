/*
 * A334897  a(n) is the number of positive integers less than or equal to n
 *          that can be written as the geometric mean of two different
 *          positive integers less than or equal to n.
 * https://oeis.org/A334897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the positive integers m≤n such that m is the
 *            geometric mean of two distinct positive integers ≤n. Algorithm:
 *            for each m (2≤m≤n) factor m, enumerate all divisors d of m² with
 *            d<m, and check if the paired divisor b=m²/d satisfies b≤n.
 *            Because d<m≤n, only the bound on b remains to be tes...
 * Range:     u8 n <= 31, u16 n <= 519, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gm_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_gm_candidate.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t m = 2; m <= n; ++m)
        if (_gm_candidate(m, n))
            ++cnt;
    return cnt;
}
