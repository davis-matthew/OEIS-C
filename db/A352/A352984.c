/*
 * A352984  a(n) = Sum_{k=0..floor(n/3)} k^(3*n).
 * https://oeis.org/A352984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} k^(3*n)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Require the big integer power function from the utilities. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;          /* floor(n/3) */
    seq_t sum = 0;
    seq_t exp = (seq_t)n * 3;    /* exponent 3*n */

    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term;
        if (k == 0) {
            /* 0^0 is defined as 1; otherwise 0. */
            term = (exp == 0) ? 1 : 0;
        } else {
            term = big_pow((seq_t)k, exp);
        }
        sum += term;
    }
    return sum;
}
