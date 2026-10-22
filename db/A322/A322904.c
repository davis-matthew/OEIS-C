/*
 * A322904  a(n) = Sum_{k=0..n} binomial(2*n+1,2*k+1)*(n^2-1)^(n-k)*n^(2*k).
 * https://oeis.org/A322904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(2*n+1,2*k+1) * (n^2-1)^(n-k) *
 *            n^(2*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    /* Convert n to the large‑integer type */
    seq_t n_seq       = (seq_t)n;
    seq_t n_sq_minus1 = n_seq * n_seq - (seq_t)1;   /* n^2 - 1 */

    /* 2*n + 1 is the upper bound for the binomial coefficient */
    idx_t two_n_plus_one = 2 * n + 1;
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t two_k_plus_one = 2 * k + 1;

        /* C(2*n+1, 2*k+1) */
        seq_t bin = binomial(two_n_plus_one, two_k_plus_one);

        /* (n^2-1)^(n-k)  and  n^(2*k)  */
        seq_t pw1 = big_pow(n_sq_minus1, (seq_t)(n - k));
        seq_t pw2 = big_pow(n_seq,         (seq_t)(2 * k));

        /* Multiply terms and accumulate */
        seq_t term = bin * pw1;
        term = term * pw2;
        result += term;
    }

    return result;
}
