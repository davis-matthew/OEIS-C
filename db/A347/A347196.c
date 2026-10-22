/*
 * A347196  Let c(k) be the infinite binary string 010111001... (A030308), the
 *          concatenation of reverse order integer binary words (
 *          0;1;01;11;001;101;... ). a(n) is the bit index k of the first
 *          occurrence of the reverse order binary word of n ( n = 2^0*c(a(n))
 *          + 2^1*c(a(n)+1) + ... ).
 * https://oeis.org/A347196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = minimal index k such that the reversed binary *
 *            representation of n occurs as a contiguous substring of the *
 *            infinite binary word c = 010111001... (A030308)
 * Range:     u8 n <= 63, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* length of binary representation of x, counting the leading 1,
 * with x==0 yielding 1. */
static inline idx_t bitlen_idx(idx_t x) {
    idx_t len = 0;
    do {
        ++len;
        x >>= 1;
    } while (x);
    return len;
}

seq_t a(idx_t n) {
    /* length of the reversed binary word of n */
    idx_t len_rev = bitlen_idx(n);

    /* position of word n in the concatenation c */
    idx_t pos_n = 0;
    for (idx_t m = 0; m < n; ++m) {
        pos_n += bitlen_idx(m);
    }

    /* we need a prefix of length pos_n + len_rev to contain the first occurrence */
    size_t total_len = (size_t)(pos_n + len_rev);

    /* allocate buffer for bits of c */
    char *buf = (char *)malloc(total_len);
    if (!buf) return (seq_t)0;           /* unlikely on the tests */

    /* generate bits of c up to total_len */
    size_t idx = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t len_m = bitlen_idx(m);
        for (idx_t i = 0; i < len_m && idx < total_len; ++i) {
            buf[idx++] = (char)((m >> i) & 1);   /* LSB first (reversed word) */
        }
        if (idx >= total_len) break;
    }

    /* search for the first occurrence of reversed bits of n */
    seq_t result = (seq_t)-1;
    for (size_t start = 0; start + len_rev <= total_len; ++start) {
        unsigned matched = 1;
        for (idx_t i = 0; i < len_rev; ++i) {
            if (buf[start + i] != (char)((n >> i) & 1)) {
                matched = 0;
                break;
            }
        }
        if (matched) {
            result = (seq_t)start;
            break;
        }
    }

    free(buf);
    return result;
}
