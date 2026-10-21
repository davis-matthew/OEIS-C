/*
 * A274265  a(n) = (3*n - 1)^(n-1).
 * https://oeis.org/A274265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        /* behavior for n < 1 is irrelevant */
        return (seq_t)0;
    }
    seq_t base = (seq_t)n * (seq_t)3 - (seq_t)1;
    seq_t exp  = (seq_t)(n - (idx_t)1);
    return big_pow(base, exp);
}
