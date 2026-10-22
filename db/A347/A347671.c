/*
 * A347671  a(n) = n^n mod 100.
 * https://oeis.org/A347671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^n mod 100
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 where the convention is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Reduce the base modulo 100 to keep intermediate values small. */
    seq_t base = (seq_t)(n % 100);

    /* Compute base^n mod 100 using efficient modular exponentiation. */
    return mod_pow(base, (uint64_t)n, (seq_t)100);
}
