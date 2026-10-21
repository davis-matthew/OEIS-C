/*
 * A290029  Number of 6-cycles in the n-halved cube graph.
 * https://oeis.org/A290029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*binomial(n, 4)*(81*n^2 - 552*n + 952)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Promote n to seq_t for arithmetic that involves seq_t operands. */
    seq_t nn = (seq_t)n;

    /* 2^n */
    seq_t pow = pow2(n);

    /* binomial(n,4) is 0 when n < 4, so the result is 0 for n = 1,2,3. */
    seq_t bin = binomial(n, 4);

    /* Polynomial part: 81*n^2 - 552*n + 952 */
    seq_t poly = (seq_t)81 * nn * nn - (seq_t)552 * nn + (seq_t)952;

    return pow * bin * poly;
}
