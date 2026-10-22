/*
 * A369026  a(n) = floor(n^(n - 1) / 2) for n > 0 and otherwise 0.
 * https://oeis.org/A369026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(n - 1) / 2) for n > 0, otherwise 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* For n > 0 compute n^(n-1) and divide by 2 (floor). */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)(n - 1);
    seq_t value = big_pow(base, exp);
    value /= 2;            /* integer division floors automatically */
    return value;
}
