/*
 * A305156  a(n) = 136*2^n - 78 (n>=0).
 * https://oeis.org/A305156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 136 * 2^n - 78 (n >= 0)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 136 * 2^n - 78.  seq_pow produces 2^n with arbitrary
       precision when seq_t is such.  The multiplication and subtraction
       are performed in the type seq_t so that overflow only occurs
       when seq_t cannot hold the result (as required for the 64‑bit
       build). */
    seq_t val = seq_pow(2, n);    /* val = 2^n */
    val = 136 * val;              /* val = 136 * 2^n */
    val -= 78;                    /* val = 136 * 2^n - 78 */
    return val;
}
