/*
 * A393976  a(n) = phi(n)^2 + phi(n) + 1, where phi is Euler's totient
 *          function.
 * https://oeis.org/A393976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n)^2 + φ(n) + 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       to keep the function defined for all idx_t values, even though
       the OEIS offset is 1. */
    if (n == 0)
        return 0;

    /* Convert the index to the seq_t type to use with the helper
       functions. */
    seq_t sn = (seq_t)n;

    /* Compute φ(n) using the supplied helper. */
    seq_t phi = calc_phi(sn);

    /* Compute φ(n)^2 + φ(n) + 1.  All operations are on seq_t, so
       they respect the bit‑width of the build (8/16/32/64/128 or big
       int). */
    seq_t result = phi * phi + phi + (seq_t)1;
    return result;
}
