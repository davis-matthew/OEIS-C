/*
 * A246170  Beatty sequence for sqrt(14).
 * https://oeis.org/A246170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sqrt(14))
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for correct arithmetic. */
    seq_t ns = (seq_t)n;

    /* Compute n^2. */
    seq_t sq = seq_mul(ns, ns);

    /* Multiply by 14. */
    sq = seq_mul(sq, (seq_t)14);

    /* Return floor(sqrt(14 * n^2)) == floor(n * sqrt(14)). */
    return floor_sqrt(sq);
}
