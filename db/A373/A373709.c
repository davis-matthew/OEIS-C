/*
 * A373709  Partial sums of A119387.
 * https://oeis.org/A373709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n+2)*d - 2*n - 2^d + p - 1 where d = bit_width(n+1) =
 *            floor_log2_uint64(n+1)+1 and p = popcount(n+1)
 * Range:     u8 n <= 73, u16 n <= 6699, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   floor_log2_uint64, popcount_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/popcount_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t np1 = n + 1;
    /* bit width of n+1 */
    idx_t d = floor_log2_uint64(np1) + 1;
    /* number of 1‑bits in n+1 */
    int p = popcount_u64(np1);

    seq_t result = ((seq_t)n + 2) * (seq_t)d;
    result -= (seq_t)n * 2;
    result -= pow2(d);
    result += (seq_t)p;
    result -= 1;

    return result;
}
