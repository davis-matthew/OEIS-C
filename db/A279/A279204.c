/*
 * A279204  Numbers whose decimal expansion is a concatenation of 4
 *          consecutive increasing nonnegative numbers.
 * https://oeis.org/A279204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concat(n, n+1, n+2, n+3)
 * Range:     u8 none, u16 n <= 6, u32 n <= 97, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_len_seq.h"

static inline seq_t pow10_seq(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t i = 0; i < 4; ++i) {
        seq_t x = (seq_t)n + (seq_t)i;
        int d = digit_len_seq(x);
        seq_t pow = pow10_seq((idx_t)d);
        result = result * pow + x;
    }
    return result;
}
