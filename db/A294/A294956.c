/*
 * A294956  a(n) = Sum_{d|n} d^(d + n/d).
 * https://oeis.org/A294956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d + n/d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 367 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t paired = n / d;
            idx_t exp = d + paired;
            total += ipow_idx((seq_t)d, exp);
            if (paired != d)
                total += ipow_idx((seq_t)paired, exp);
        }
    }
    return total;
}
