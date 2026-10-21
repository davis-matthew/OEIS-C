/*
 * A286606  a(n) = n mod product of nonzero digits of n in factorial base.
 * https://oeis.org/A286606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n mod product of nonzero digits of n expressed in
 *            factorial base
 * Range:     u8 n <= 685, u16 n <= 9630, u32 n <= 9630, u64 n <= 9630,
 *            u128 n <= 9630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t product = 1;          /* product of non-zero factorial digits */
    idx_t rem = n;              /* remaining value to decompose */

    /* Decompose n into factorial base digits: digit_i = rem % (i+1) */
    for (idx_t i = 1; rem != 0; ++i) {
        idx_t digit = rem % (i + 1);
        if (digit != 0) {
            product *= (seq_t)digit;
        }
        rem /= (i + 1);
    }

    /* Return n modulo that product */
    return (seq_t)n % product;
}
