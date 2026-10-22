/*
 * A364083  Expansion of Sum_{k>0} k * x^k / (1 - x^(4*k-3)).
 * https://oeis.org/A364083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Sum_{d | 4*n-3, d==1 (mod 4)} (d+3)
 * Range:     u8 n <= 16, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 4*n - 3 (always odd) */
    seq_t m = (seq_t)n * 4 - 3;
    seq_t sum = 0;

    /* upper bound for divisor test */
    seq_t sq = floor_sqrt(m);
    idx_t limit = (idx_t)sq;          /* trabable, as m is small for used n */

    for (idx_t d = 1; d <= limit; ++d) {
        seq_t dseq = (seq_t)d;
        if (m % dseq == 0) {
            if (dseq % 4 == 1)   sum += dseq + 3;

            seq_t d2seq = m / dseq;        /* complementary divisor */
            if (d2seq != dseq && d2seq % 4 == 1)
                sum += d2seq + 3;
        }
    }
    return sum / 4;
}
