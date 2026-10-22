/*
 * A372370  Coefficient of x^n in the expansion of ( (1+x+x^2)^2 / (1+x) )^n.
 * https://oeis.org/A372370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n,k) *
 *            binomial(n-k,n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t two_n = n + n;          /* 2*n */
    idx_t max_k = n >> 1;        /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term1 = binom(two_n, k);                /* binomial(2*n,k)  */
        seq_t term2 = binom(n - k, n - (k << 1));     /* binomial(n-k,n-2*k) */
        result += term1 * term2;
    }
    return result;
}
