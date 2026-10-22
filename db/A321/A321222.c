/*
 * A321222  a(n) = Sum_{d|n} mu(d)*d^n.
 * https://oeis.org/A321222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{p|n} (1 - p^n) (OEIS A321222)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 */
    if (temp % 2 == 0) {
        idx_t p = 2;
        seq_t pow = big_pow((seq_t)p, (seq_t)n);
        seq_t term = 1 - pow;
        result *= term;
        while (temp % 2 == 0) temp /= 2;
    }

    /* odd factors */
    for (idx_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            idx_t p = d;
            seq_t pow = big_pow((seq_t)p, (seq_t)n);
            seq_t term = 1 - pow;
            result *= term;
            while (temp % d == 0) temp /= d;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        idx_t p = temp;
        seq_t pow = big_pow((seq_t)p, (seq_t)n);
        seq_t term = 1 - pow;
        result *= term;
    }

    return result;
}
