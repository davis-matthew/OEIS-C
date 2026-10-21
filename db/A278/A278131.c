/*
 * A278131  a(n) = 591*2^n - 273.
 * https://oeis.org/A278131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 591*2^n - 273
 * Range:     u8 none, u16 n <= 6, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A278131. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);                 /* val = 2^n */
    val *= (seq_t)591;                   /* val = 591*2^n */
    val -= (seq_t)273;                   /* val = 591*2^n - 273 */
    return val;
}
