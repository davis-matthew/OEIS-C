/*
 * A341197  a(n) = Sum_{k=0..n} k^n * (n-k)! * binomial(n,k)^2.
 * https://oeis.org/A341197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^n * (n-k)! * binomial(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq, big_pow, binomial, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Pre‑compute factorials 0! … n! */
    seq_t *fact = alloc_seq((size_t)n + 1);
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[(size_t)i] = fact[(size_t)(i - 1)] * (seq_t)i;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t powk;
        if (k == 0 && n > (idx_t)0)
            powk = (seq_t)0;                 /* 0^n = 0 for n>0   */
        else
            powk = big_pow((seq_t)k, (seq_t)n);

        seq_t bin = binomial(n, k);
        seq_t term = powk * fact[(size_t)(n - k)];
        term *= bin;
        term *= bin;

        sum += term;
    }

    free_seq(fact);
    return sum;
}
