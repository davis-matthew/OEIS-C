/*
 * A248365  4n concatenated with itself.
 * https://oeis.org/A248365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(4*n,4*n) = 4*n * 10^d + 4*n, where d =
 *            digit_len(4*n)
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* compute 4*n as seq_t */
    seq_t val = (seq_t)n * (seq_t)4;
    /* number of decimal digits in val */
    int d = digit_len(val);
    /* 10^d as seq_t */
    seq_t pow = pow10((idx_t)d);
    /* concatenate val with itself */
    return val * pow + val;
}
