/*
 * A400515  a(n) = (16^n + 3*4^n - 4*7^n)/12.
 * https://oeis.org/A400515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = (16^n + 3*4^n - 4*7^n)/12 (n >= 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Convert the index to the sequence type for exponentiation */
    seq_t exp = (seq_t)n;

    /* Compute the three required powers using fast exponentiation */
    seq_t p16 = big_pow((seq_t)16, exp);
    seq_t p4  = big_pow((seq_t)4,  exp);
    seq_t p7  = big_pow((seq_t)7,  exp);

    /* Assemble the numerator: 16^n + 3*4^n - 4*7^n */
    seq_t num = p16 + (seq_t)3 * p4 - (seq_t)4 * p7;

    /* Integer division by 12 gives the term a(n) */
    return num / (seq_t)12;
}
