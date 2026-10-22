/*
 * A377730  Number of integers less than n that have the same greatest prime
 *          factor as n.
 * https://oeis.org/A377730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{j < n : gpf(j) = gpf(n)}|
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t gp = gpf((uint64_t)n);
    seq_t cnt = 0;
    for (idx_t j = 2; j < n; ++j)
    {
        if (gpf((uint64_t)j) == gp)
            ++cnt;
    }
    return cnt;
}
