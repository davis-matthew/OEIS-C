/*
 * A244004  a(n) = Sum_{k=0..n} binomial(n,k) * (1 + 3^k)^k.
 * https://oeis.org/A244004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (1 + 3^k)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);
        seq_t pow3k = pow3(k);
        seq_t base = pow3k + 1;
        seq_t pow = big_pow(base, (seq_t)k);
        sum += bin * pow;
    }
    return sum;
}
