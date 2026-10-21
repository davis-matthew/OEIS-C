/*
 * A270593  Total number of subtrees of the complete simple undirected graph
 *          K_n on n vertices.
 * https://oeis.org/A270593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * k^(k-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Handles k=1 by defining 1^(−1)=1, as the number of trees on 1 vertex is 1. */

#include "../utils/big_pow.h"      /* big_pow(seq_t base, seq_t exp) */
#include "../utils/binomial.h"     /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_mul.h"      /* seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t comb = binomial(n, k);
        seq_t pow_val;

        if (k <= 2)                /* 1^(−1) = 1, 2^0 = 1 */
            pow_val = 1;
        else
            pow_val = big_pow((seq_t)k, (seq_t)(k - 2));

        seq_t term = seq_mul(comb, pow_val);
        sum += term;
    }
    return sum;
}
