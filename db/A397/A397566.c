/*
 * A397566  a(0) = 0; for n >= 1, a(n) is the smallest number whose Hamming
 *          distance from a(n-1) is n^2.
 * https://oeis.org/A397566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by the following block pattern. For odd n the
 *            binary representation consists of blocks of lengths 1,5,9,13,…
 *            (4*i‑3) starting at bit 0, with gaps of lengths 3,7,11,… (4*i‑1)
 *            between consecutive blocks. For even n the block lengths are
 *            3,7,11,15,… (4*i‑1) starting at bit 1, with g...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    bool odd   = (n & 1ULL);
    idx_t blocks = (n + 1) / 2;       /* number of blocks */
    seq_t result = 0;
    idx_t start  = odd ? 0 : 1;       /* start of the first block */

    for (idx_t i = 1; i <= blocks; ++i) {
        idx_t len  = odd ? 4 * i - 3 : 4 * i - 1;           /* block length */
        seq_t block = pow2(start) * (pow2(len) - 1);        /* 2^start * (2^len − 1) */
        result += block;

        idx_t gap = odd ? 4 * i - 1 : 4 * i + 1;           /* gap after this block */
        start += len + gap;                                 /* next block start */
    }
    return result;
}
