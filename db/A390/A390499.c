/*
 * A390499  a(n) = Product_{k=0..n} binomial(n + 2*k, k).
 * https://oeis.org/A390499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} binomial(n + 2*k, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Computes the n-th term of OEIS sequence A390499. */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binomial(n + 2 * k, k);
        result = result * term;
    }
    return result;
}
