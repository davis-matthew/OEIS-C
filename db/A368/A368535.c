/*
 * A368535  a(n) = Sum_{k=1..n} binomial(k+2,3) * n^(n-k).
 * https://oeis.org/A368535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+2,3) * n^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Convert index to seq_t and pre‑compute the first power n^(n-1) */
    seq_t sn     = (seq_t)n;
    seq_t power  = big_pow(sn, sn - (seq_t)1);   /* n^(n-1)   */
    seq_t result = (seq_t)0;

    /* Iterate k = 1 … n, reducing the power by division by n each step */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binomial(k + 2, 3);  /* C(k+2,3)   */
        result += bin * power;
        if (k < n) power /= sn;          /* n^(n-k-1)  */
    }

    return result;
}
