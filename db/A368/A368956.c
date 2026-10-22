/*
 * A368956  a(n) = 3^n - 2^(n+1) - 1.
 * https://oeis.org/A368956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - 2^(n+1) - 1
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, safe fallback */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n + (seq_t)1);
    return pow3 - pow2 - (seq_t)1;
}
