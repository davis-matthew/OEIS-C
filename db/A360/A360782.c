/*
 * A360782  Expansion of Sum_{k>=0} x^k / (1 - k*x^2)^(k+1).
 * https://oeis.org/A360782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^k * binomial(n-k,k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t base = (seq_t)(n - 2 * k);
        seq_t pow_val = big_pow(base, (seq_t)k);
        seq_t bin = binomial(n - k, k);
        sum += pow_val * bin;
    }
    return sum;
}
