/*
 * A341330  a(n) = Sum_{k=1..n} (-k)^(k+1).
 * https://oeis.org/A341330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-k)^(k+1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = big_pow((seq_t)k, (seq_t)(k + 1));  /* k^(k+1) */
        if ((k & 1) == 0)    /* k even → sign negative */
            sum -= term;
        else                 /* k odd  → sign positive */
            sum += term;
    }
    return sum;
}
