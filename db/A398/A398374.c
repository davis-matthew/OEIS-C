/*
 * A398374  Number of permutations that give the maximum number of comparisons
 *          for sorting n elements by binary insertions.
 * https://oeis.org/A398374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*(2*n - 2^ceil(log_2(n))), a(0)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t ceil;
        if (i == 1) {
            ceil = 0;
        } else {
            ceil = floor_log2_uint64(i - 1) + 1;
        }

        seq_t two_n = ((seq_t)i) * 2;
        seq_t pow = pow2(ceil);
        seq_t factor = two_n - pow;
        result *= factor;
    }

    return result;
}
