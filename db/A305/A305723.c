/*
 * A305723  Crystal ball sequence for the lattice C_9.
 * https://oeis.org/A305723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..9} binomial(18, 2*k) * binomial(n + k, 9)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 12, u64 n <= 154,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Computes the n-th term of OEIS sequence A305723.  The function
 * requires the helper ``binomial`` from ../utils/binomial.h.
 * It works for arbitrary-precision seq_t as well as for 64‑bit
 * unsigned types; for the 64‑bit build it will be correct
 * whenever the result fits in seq_t. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= 9; ++k) {
        seq_t coeff = binomial(18, 2 * k);
        seq_t term  = binomial(n + k, 9);
        sum += coeff * term;
    }
    return sum;
}
