/*
 * A300517  a(n) is the concatenation of n n times, n-1 n-1 times, ..., 22, 1.
 * https://oeis.org/A300517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat of i repeated i times for i = n down to 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for n=0, although offset is 1 */

    seq_t result = (seq_t)0;

    for (idx_t i = n; i > (idx_t)0; --i) {
        /* number of decimal digits of i */
        idx_t d = digit_count((uint64_t)i);

        /* 10^d and 10^(d*i)  (both seq_t) */
        seq_t pow_d   = pow10(d);
        seq_t pow_di  = pow10(d * i);

        /* value of i repeated i times:
           block = i * (10^(d*i) – 1) / (10^d – 1)   */
        seq_t numer = pow_di - (seq_t)1;
        seq_t denom = pow_d - (seq_t)1;
        seq_t block = ((seq_t)i * numer) / denom;

        /* append block to the right of current result */
        seq_t shift = pow_di;                 /* 10^(d*i) */
        result = result * shift + block;
    }

    return result;
}
