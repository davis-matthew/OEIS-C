/*
 * A295934  Number of (not necessarily maximal) cliques in the n-odd graph.
 * https://oeis.org/A295934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n - 1, n) + n*binomial(2*n, n)/4 + 1, n>2
 *            a(1)=2, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    /* term1 = binomial(2*n - 1, n) */
    seq_t term1 = binom((idx_t)(2*n - 1), n);

    /* term2 = n * binomial(2*n, n) / 4 */
    seq_t term2 = binom((idx_t)(2*n), n);
    term2 = (seq_t)n * term2;   /* multiply by n */
    term2 = term2 / 4;          /* divide by 4 */

    return term1 + term2 + (seq_t)1;
}
