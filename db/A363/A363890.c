/*
 * A363890  Sum of divisors of 3*n-1 of form 3*k+2.
 * https://oeis.org/A363890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | (3*n-1), d ≡ 2 (mod 3)} d
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_special_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sum_special_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* behaviour below the offset is irrelevant */

    uint64_t m = 3 * (uint64_t)n - 1;
    return sum_special_divisors(m);
}
