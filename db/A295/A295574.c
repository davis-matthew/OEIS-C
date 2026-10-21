/*
 * A295574  a(n) = Sum_{1 <= j <= n/2, gcd(j,n)=1} j^2.
 * https://oeis.org/A295574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= j <= n/2, gcd(j,n)=1} j^2
 * Range:     u8 n <= 18, u16 n <= 120, u32 n <= 4690, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;
    for (idx_t j = 1; j <= limit; ++j) {
        if (gcd((seq_t)j, (seq_t)n) == 1) {
            seq_t sq = (seq_t)j * (seq_t)j;
            sum += sq;
        }
    }
    return sum;
}
