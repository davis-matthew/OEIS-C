/*
 * A376491  G.f. satisfies A(x) = 1 / (1 - x^4*A(x)^4 / (1 - x)).
 * https://oeis.org/A376491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(5*k,k) *
 *            binomial(n-3*k-1,n-4*k) / (4*k+1)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t acc = 0;
    idx_t maxk = n / 4;

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t t1 = binom(5 * k, k);
        seq_t t2 = binom(n - 3 * k - 1, n - 4 * k);
        seq_t prod = t1 * t2;
        seq_t denom = (seq_t)(4 * k + 1);
        acc += prod / denom;
    }

    return acc;
}
