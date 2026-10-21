/*
 * A290651  a(n) = 5 - 2^(n - 1) + 3^(n - 1) + n - 2.
 * https://oeis.org/A290651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) - 2^(n-1) + n + 3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Computes A290651:  a(n) = 5 - 2^(n-1) + 3^(n-1) + n - 2
    (which simplifies to 3^(n-1) - 2^(n-1) + n + 3)
    The function works for any seq_t and idx_t provided by the build.
    No external tables or precomputation are used. */
seq_t a(idx_t n)
{
    if (n == 0)        /* Undefined for the offset, but safe for any call */
        return (seq_t)0;

    idx_t exp = n - 1;          /* exponent for the powers of 2 and 3 */

    /* Compute 2^(n-1) and 3^(n-1) using direct multiplication.
       For arbitrary‑precision builds the operators handle large values.
       For 64‑bit builds the result is correct for all n where the
       final value fits in 64 bits; larger n overflow but are not tested. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= (seq_t)2;

    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow3 *= (seq_t)3;

    /* a(n) = pow3 - pow2 + n + 3 */
    seq_t result = pow3 - pow2;
    result += (seq_t)n;
    result += (seq_t)3;

    return result;
}
