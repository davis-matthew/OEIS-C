/*
 * A296354  Official position where binary expansion of n starts in the list
 *          of binary numbers in the binary Champernowne sequence A076478.
 * https://oeis.org/A296354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{len=1}^{k-1} len * 2^len + k * n * where k is the
 *            bit‑length of n (k = 1 for n = 0)
 * Range:     u8 n <= 31, u16 n <= 2047, u32 n <= 9366, u64 n <= 9366,
 *            u128 n <= 9366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute bit‑length k of n (k = 1 for n = 0) */
    idx_t k = 0;
    idx_t t = n;
    while (t) {
        k++;
        t >>= 1;
    }
    if (k == 0) k = 1;   /* handle n == 0 */

    /* prefix sum: sum_{len=1}^{k-1} len * 2^len */
    seq_t prefix = 0;
    for (idx_t len = 1; len < k; ++len) {
        prefix += ((seq_t)len) << len;
    }

    return prefix + ((seq_t)n) * k;
}
