/*
 * A264618  Working in binary, write n followed by 0 then n-reversed
 *          (including leading zeros); show result in base 10.
 * https://oeis.org/A264618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n << (bitlen(n)+1)) | rev(n)
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 32767, u64 n <= 32767,
 *            u128 n <= 32767, big any n
 * Validated: standard, pass, 73 terms through n = 9340 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bitlen(n) is the number of bits of n (1 for n = 0),
   and rev(n) is the reflection of the low bitlen(n) bits of n. */

#include <stdint.h>

static inline seq_t reverse_bits_seq(seq_t n, unsigned len) {
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        rev = (rev << 1) | ((n >> i) & 1);
    }
    return rev;
}

static inline unsigned bitlen_idx(idx_t n) {
    unsigned len = 0;
    if (n == 0) return 1;          /* treat 0 as length 1 ("0") */
    while (n) {
        n >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n) {
    unsigned len = bitlen_idx(n);
    seq_t sn   = (seq_t)n;
    seq_t rev  = reverse_bits_seq(sn, len);
    seq_t res  = (sn << (len + 1)) | rev;
    return res;
}
