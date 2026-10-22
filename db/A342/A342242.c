/*
 * A342242  For any n > 0, a(n) is the least positive number whose binary
 *          expansion is both a prefix and a suffix of the binary expansion of
 *          n; a(0) = 0.
 * https://oeis.org/A342242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least positive integer whose binary expansion is both a
 *            prefix and a suffix of the binary expansion of n; a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned bitlen_idx(idx_t x) {
    unsigned len = 0;
    while (x) { ++len; x >>= 1; }
    return len;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    unsigned L = bitlen_idx(n);
    for (unsigned l = 1; l <= L; ++l) {
        uint64_t prefix = ((uint64_t)n) >> (L - l);
        uint64_t mask   = (l == 64) ? UINT64_MAX : ((uint64_t)1 << l) - 1;
        uint64_t suffix = ((uint64_t)n) & mask;
        if (prefix == suffix) return (seq_t)prefix;
    }
    /* The full string is always a border, so this line is unreachable. */
    return (seq_t)n;
}
