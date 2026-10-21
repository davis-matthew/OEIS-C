/*
 * A246159  Inverse function to the injection A048724.
 * https://oeis.org/A246159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * A010059(n) * A006068(n) * It follows that a(n) is
 *            the index of the Gray‑coded integer n * when only those n with
 *            an even number of 1‑bits are kept. * If popcount(n) is odd, a(n)
 *            = 0. Otherwise a(n) = (gray_to_bin(n) >> 1)
 * Range:     u8 n <= 512, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   gray_to_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gray_to_bin.h"

seq_t a(idx_t n)
{
    /* If n has an odd number of 1's, it never appears in the
     * injection A048724; hence a(n) = 0. */
    if ((__builtin_popcountll((unsigned long long)n) & 1) != 0)
        return 0;

    /* m is the binary index whose Gray code equals n.
     * Since popcount(n) is even, m is even, so m>>1 equals
     * the desired index in the subsequence of even‑popcount Gray
     * codes. */
    idx_t m = gray_to_bin(n);
    return (seq_t)(m >> 1);
}
