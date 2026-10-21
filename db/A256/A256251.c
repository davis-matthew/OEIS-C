/*
 * A256251  First differences of A256250.
 * https://oeis.org/A256251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 4*A006257(n), i.e. a(n) = 8*(n - 2^⌊log₂(n)⌋) + 4 (n ≥ 0)
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_u64(idx_t n) {
    /* Return ⌊log₂(n)⌋; n > 0 */
    idx_t p = 0;
    while ((idx_t)1 << (p + 1) <= n) {
        ++p;
    }
    return p;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* Highest power of two ≤ n */
    idx_t p = floor_log2_u64(n);
    idx_t pow2 = (idx_t)1 << p;

    seq_t diff = (seq_t)(n - pow2);
    return (seq_t)(8 * diff + 4);
}
