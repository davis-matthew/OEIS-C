/*
 * A372117  a(n) = Product_{k=0..n} binomial(n+k, k)^k.
 * https://oeis.org/A372117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0..n} binomial(n+k, k)^k
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"       /* binom(idx_t, idx_t) */
#include "../utils/big_pow.h"     /* big_pow(seq_t, seq_t) */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binom(n + k, k);
        if (k == 0u)          /* term = 1 */
            continue;
        seq_t term = big_pow(bin, (seq_t)k);
        result = seq_mul(result, term);
    }
    return result;
}
