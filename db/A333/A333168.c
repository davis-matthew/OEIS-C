/*
 * A333168  a(n) = Sum_{k=0..n} r_2(k^2 + 1), where r_2(k) is the number of
 *          ways of writing k as a sum of 2 squares (A004018).
 * https://oeis.org/A333168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^n r_2(k^2 + 1)
 * Range:     u8 n <= 22, u16 n <= 2862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   r2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/r2_uint64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        uint64_t m = (uint64_t)k * (uint64_t)k + 1ULL;
        sum += r2_uint64(m);
    }
    return sum;
}
