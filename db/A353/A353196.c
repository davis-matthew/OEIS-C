/*
 * A353196  Number of stabilizer states on n qubits.
 * https://oeis.org/A353196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*Product_{i=1..n} (2^i+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Start with the 2^n factor. */
    seq_t result = pow2(n);

    /* Multiply by (2^i + 1) for each i = 1 .. n. */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t fac = pow2(i) + (seq_t)1;
        result = result * fac;
    }
    return result;
}
