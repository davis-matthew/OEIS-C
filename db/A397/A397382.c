/*
 * A397382  a(n) = Sum_{k=0..n} binomial(n+3*k,4*k) * (binomial(n+4*k,k) +
 *          binomial(n+4*k,k-1)).
 * https://oeis.org/A397382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+3*k,4*k) * ( binomial(n+4*k,k) +
 *            binomial(n+4*k,k-1) ) where binomial(n,k) = n!/(k!(n-k)!) with
 *            the convention binomial(n,-1)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = binomial(n + 3 * k, 4 * k);
        seq_t bin2 = binomial(n + 4 * k, k);
        seq_t bin3 = 0;
        if (k > 0) bin3 = binomial(n + 4 * k, k - 1);
        seq_t term = bin1 * (bin2 + bin3);
        sum += term;
    }
    return sum;
}
