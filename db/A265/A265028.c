/*
 * A265028  First differences of A264618.
 * https://oeis.org/A265028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A264618(n) - A264618(n-1), where A264618(m) = (m <<
 *            (bitlen(m)+1)) | rev(m)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 32767, u64 n <= 32767,
 *            u128 n <= 32767, big any n
 * Validated: standard, pass, 75 terms through n = 9923 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t A265028_rev(seq_t n, unsigned len) {
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        rev = (rev << 1) | ((n >> i) & 1);
    }
    return rev;
}

static inline unsigned A265028_bitlen(idx_t n) {
    unsigned len = 0;
    if (n == 0) return 1;
    while (n) {
        n >>= 1;
        ++len;
    }
    return len;
}

static seq_t A264618_a(idx_t n) {
    unsigned len = A265028_bitlen(n);
    seq_t sn = (seq_t)n;
    seq_t rev = A265028_rev(sn, len);
    return (sn << (len + 1)) | rev;
}

seq_t a(idx_t n) {
    return A264618_a(n) - A264618_a(n - 1);
}
