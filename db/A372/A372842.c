/*
 * A372842  a(n) is the number of parking functions of order n for which the
 *          second spot is lucky.
 * https://oeis.org/A372842
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/4)*(n+1)^(n-1) - (1/4)*(n-1)^(n-1) (valid for n >= 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 2) {
        /* Undefined for n < 2; return 0 as safe default */
        return (seq_t)0;
    }

    /* Compute powers (n+1)^(n-1) and (n-1)^(n-1) */
    seq_t pow1 = big_pow((seq_t)(n + 1), (seq_t)(n - 1));
    seq_t pow2 = big_pow((seq_t)(n - 1), (seq_t)(n - 1));

    /* Numerator: 3*(n+1)^(n-1) - (n-1)^(n-1) */
    seq_t num = 3 * pow1 - pow2;

    /* Division by 4 (safe since the result is always an integer) */
    return num / (seq_t)4;
}
