/*
 * A376884  Number of binary n-sequences ending in 1 with exactly one more
 *          occurrence of 11 than 10.
 * https://oeis.org/A376884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊(n+1)/3⌋} binomial(2*k-1, k) * binomial(n-2*k,
 *            k-1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Accumulate the sum in the generic integer type seq_t.  All operations
       use the helper functions for safe arbitrary‑precision arithmetic. */
    seq_t sum = 0;
    idx_t maxk = (n + 1) / 3;

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t b1 = binomial(2 * k - 1, k);
        seq_t b2 = binomial(n - 2 * k, k - 1);
        seq_t prod = seq_mul(b1, b2);
        sum += prod;
    }

    return sum;
}
