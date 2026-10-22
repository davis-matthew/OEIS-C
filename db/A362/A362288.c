/*
 * A362288  a(n) = Product_{k=0..n} binomial(n,k)^k.
 * https://oeis.org/A362288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} binomial(n,k)^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t product = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t binom = binomial(n, k);                     /* binomial(n,k) */
        seq_t power = big_pow(binom, (seq_t)k);           /* binom^k   */
        product = seq_mul(product, power);                /* accumulate product */
    }
    return product;
}
