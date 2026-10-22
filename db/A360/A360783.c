/*
 * A360783  Expansion of Sum_{k>=0} x^k / (1 - k*x^3)^(k+1).
 * https://oeis.org/A360783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^k * binomial(n-2*k,k)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t sub = n - 3 * k;
        seq_t powv = big_pow((seq_t)sub, (seq_t)k);
        seq_t bin = binomial(n - 2 * k, k);
        res += powv * bin;
    }
    return res;
}
