/*
 * A342263  a(n) is the length of the longest substring appearing twice
 *          (possibly with overlap) in the binary expansion of n.
 * https://oeis.org/A342263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest binary substring that appears at
 *            least twice (possibly overlapping) in the binary representation
 *            of n. The implementation performs an exhaustive search over all
 *            possible substring lengths, which is sufficient because the
 *            binary length of n is at most 64 bits (idx_t is...
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) {
        return 0;
    }

    /* Compute the number of bits L in n (without leading zeros) */
    idx_t L = 0;
    idx_t temp = n;
    while (temp) {
        ++L;
        temp >>= 1;
    }

    /* Search from longest possible length down to 1 */
    for (idx_t len = L; len >= 1; --len) {
        idx_t substrCnt = L - len + 1;
        uint64_t vals[64];                /* L ≤ 64, so this is safe */

        /* Extract all substrings of this length */
        for (idx_t i = 0; i < substrCnt; ++i) {
            uint64_t mask;
            if (len == 64) {
                /* len == 64 implies L == 64 and i == 0 only */
                mask = (uint64_t)n;
            } else {
                /* Take the lowest 'len' bits after shifting right */
                mask = (uint64_t)n >> (L - len - i);
                mask &= ((1ULL << len) - 1);
            }
            vals[i] = mask;
        }

        /* Check for duplicates */
        for (idx_t i = 0; i < substrCnt; ++i) {
            for (idx_t j = i + 1; j < substrCnt; ++j) {
                if (vals[i] == vals[j]) {
                    return (seq_t)len;
                }
            }
        }
    }

    /* If no substring repeats, the answer is 0 */
    return 0;
}
