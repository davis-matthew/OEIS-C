/*
 * A349639  a(n) = Sum_{k=0..n} binomial(n,k) * A000108(k) * k^k.
 * https://oeis.org/A349639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Catalan(k) * k^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binom, catalan, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/catalan.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin   = binom(n, k);                 /* C(n,k)            */
        seq_t cat   = catalan(k);                  /* Catalan(k)        */
        seq_t powk;
        if (k == 0)
            powk = 1;                             /* 0^0 is defined as 1 */
        else
            powk = big_pow((seq_t)k, (seq_t)k);    /* k^k               */
        seq_t prod = seq_mul(bin, cat);            /* bin*cat           */
        prod = seq_mul(prod, powk);                /* (bin*cat)*k^k     */
        sum += prod;                               /* accumulate        */
    }
    return sum;
}
