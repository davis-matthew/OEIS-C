/*
 * A356806  a(n) = Sum_{k=0..n} (k*n-1)^(n-k) * binomial(n,k).
 * https://oeis.org/A356806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k*n-1)^(n-k) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = (seq_t)0;
    idx_t k;

    for (k = 1; k <= n; ++k) {
        seq_t base   = (seq_t)((seq_t)k * (seq_t)n - (seq_t)1);
        seq_t exp    = (seq_t)(n - k);
        seq_t pow_term = big_pow(base, exp);
        seq_t term = binomial(n, k) * pow_term;
        sum += term;
    }

    /* account for the k = 0 term: (-1)^n = +1 if n even, -1 if odd */
    if (n & 1)        /* n is odd */
        sum -= (seq_t)1;
    else
        sum += (seq_t)1;

    return sum;
}
