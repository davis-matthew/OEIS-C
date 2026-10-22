/*
 * A382728  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 13.
 * https://oeis.org/A382728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=0..n} Prod_{d in Base13(r)} (d+1)
 * Range:     u8 n <= 24, u16 n <= 621, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_nondiv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts non‑divisible entries of Pascal’s triangle rows 0..n by Lucas’ theorem. */
#include <stdint.h>
#include "../utils/count_nondiv.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t r = 0; ; ++r)
    {
        total += count_nondiv(r);
        if (r == n) break;
    }
    return total;
}
