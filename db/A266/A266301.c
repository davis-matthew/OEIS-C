/*
 * A266301  Binary representation of the n-th iteration of the "Rule 15"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266301:
   a(n) = 1                         if n is even
   a(n) = (10*100^n - 10) / 9       if n is odd
   (Rule 15 elementary cellular automaton starting from a single ON cell). */

#include <stdbool.h>                 /* optional, harmless */
#include "../utils/big_pow.h"        /* provides: seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Even indices give the constant 1. */
    if ((n & 1u) == 0)
        return (seq_t)1;

    /* Compute 100^n.  big_pow accepts seq_t arguments. */
    seq_t pow100n = big_pow((seq_t)100, (seq_t)n);

    /* t = 10 * 100^n - 10  */
    seq_t t = pow100n * (seq_t)10;
    t -= (seq_t)10;

    /* Return (10*100^n - 10) / 9. */
    return t / (seq_t)9;
}
