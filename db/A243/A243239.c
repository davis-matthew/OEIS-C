/*
 * A243239  a(n) = 10^n mod 97.
 * https://oeis.org/A243239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n mod 97
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

/* Compute 10^n modulo 97. */
seq_t a(idx_t n)
{
    /* The modulus is small enough to fit in the native integer type of seq_t. */
    const seq_t mod = (seq_t)97U;
    return mod_pow((seq_t)10U, n, mod);
}
