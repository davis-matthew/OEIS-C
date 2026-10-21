/*
 * A249332  a(n) = Sum_{k=0..2*n} binomial(2*n, k)^4.
 * https://oeis.org/A249332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2*n} binomial(2*n, k)^4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    idx_t m = n << 1;          /* m = 2*n */
    seq_t sum = 0;             /* accumulator */

    for (idx_t k = 0; k <= m; ++k) {
        seq_t c  = binomial(m, k);      /* C(2n, k)   */
        seq_t sq = seq_mul(c, c);        /* c^2        */
        seq_t sq4 = seq_mul(sq, sq);     /* c^4        */
        sum += sq4;
    }

    return sum;
}
