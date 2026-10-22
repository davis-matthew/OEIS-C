/*
 * A349466  Expansion of 1/((1-12*x)*(1-16*x)*(1-18*x)*(1-24*x)).
 * https://oeis.org/A349466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*24^n + 64*16^n - 81*18^n - 6*12^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute each term using integer exponentiation */
    seq_t term1 = (seq_t)24 * pow_uint((uint64_t)24, n);
    seq_t term2 = (seq_t)64 * pow_uint((uint64_t)16, n);
    seq_t term3 = (seq_t)81 * pow_uint((uint64_t)18, n);
    seq_t term4 = (seq_t)6  * pow_uint((uint64_t)12, n);

    /* Combine terms according to the formula */
    seq_t result = term1;
    result += term2;
    result -= term3;
    result -= term4;

    return result;
}
