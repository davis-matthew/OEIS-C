/*
 * A368844  a(n) gives the number of triples of equally spaced 0's in the
 *          binary expansion of n (without leading zeros).
 * https://oeis.org/A368844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) counts triples of equally spaced 0s in the binary expansion
 *            of n (without leading zeros). The function is implemented by an
 *            explicit scan of the binary digits of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* n fits in uint64_t; its binary expansion has at most 64 bits. */
    if (n == 0)
        return 0;

    seq_t value = (seq_t)n;

    /* Compute the number of bits needed to represent n. */
    idx_t len = 0;
    seq_t tmp = value;
    while (tmp != 0) {
        tmp >>= 1;
        len++;
    }

    /* Store positions of zero bits.  At most len positions. */
    idx_t zero_pos[64];  /* len ≤ 64 for 64‑bit n */
    size_t zero_cnt = 0;
    for (idx_t pos = 0; pos < len; ++pos) {
        if (((value >> pos) & (seq_t)1) == 0) {
            zero_pos[zero_cnt++] = pos;
        }
    }

    /* Count triples i < j < k with equally spaced zeros. */
    seq_t count = 0;
    for (size_t aidx = 0; aidx < zero_cnt; ++aidx) {
        idx_t i = zero_pos[aidx];
        for (size_t bidx = aidx + 1; bidx < zero_cnt; ++bidx) {
            idx_t j = zero_pos[bidx];
            idx_t k = 2 * j - i;
            if (k < len && ((value >> k) & (seq_t)1) == 0) {
                count++;
            }
        }
    }

    return count;
}
