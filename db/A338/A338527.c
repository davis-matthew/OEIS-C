/*
 * A338527  Number of spanning trees in the join of the disjoint union of two
 *          complete graphs each on n and n+1 vertices with the empty graph on
 *          n+1 vertices.
 * https://oeis.org/A338527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)*(2 n + 2)^n*(2 n + 1)^(2 n - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t result = (seq_t)(N + 1);

    seq_t base1 = (seq_t)(2 * N + 2);
    seq_t pow1  = big_pow(base1, N);
    result *= pow1;

    seq_t base2 = (seq_t)(2 * N + 1);
    seq_t pow2  = big_pow(base2, (seq_t)(2 * N - 1));
    result *= pow2;

    return result;
}
