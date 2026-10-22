/*
 * A371017  The second row of generalized Knuth's old sum.
 * https://oeis.org/A371017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(8n - 3)*binomial(2n-2,n-1)*binomial(2n+1,2n-2)/n
 *            (valid for n≥1. Uses helper binomial(...) from
 *            ../utils/binomial.h)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               // offset is 1, but guard against n=0

    idx_t N = n;

    /* Compute binom(2n-2, n-1) */
    seq_t term = binomial(2 * N - 2, N - 1);

    /* Compute binom(2n+1, 2n-2) = binom(2n+1, 3) = (2n+1)*(2n)*(2n-1)/6 */
    seq_t term2 = (((seq_t)(2 * N + 1) * (2 * N)) * (2 * N - 1)) / 6;

    /* Product of the two binomials */
    seq_t prod = term * term2;

    /* Exact division by n */
    prod = prod / N;

    /* Multiply by 4*(8n-3) */
    seq_t multiplier = 4 * (8 * N - 3);

    return prod * multiplier;
}
