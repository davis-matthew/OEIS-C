/*
 * A392221  Sum of depths of leaves in a complete binary tree with n nodes.
 * https://oeis.org/A392221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 2^h + 1) + (h - 1) * ceil(n/2) where h =
 *            floor(log2(n))
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute h = floor(log2(n)) */
    unsigned int h = 0;
    if (n > 1) {
        idx_t temp = n;
        while (temp > 1) {
            temp >>= 1;
            h++;
        }
    }

    /* 2^h as an idx_t value, safe because h < 64 */
    idx_t power = (idx_t)1 << h;

    /* ceil(n/2) */
    seq_t ceil_n2 = (seq_t)((n + 1ULL) >> 1);

    /* (n - 2^h + 1) */
    seq_t part1 = (seq_t)n - (seq_t)power + (seq_t)1;

    if (h == 0) {
        /* When h == 0, h-1 is -1, so subtract ceil(n/2) */
        return part1 - ceil_n2;
    } else {
        /* (h - 1) * ceil(n/2) */
        seq_t part2 = (seq_t)(h - 1) * ceil_n2;
        return part1 + part2;
    }
}
