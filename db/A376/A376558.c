/*
 * A376558  E.g.f. satisfies A(x) = exp( x*A(x) / (1 - x^2*A(x)^2) ).
 * https://oeis.org/A376558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} ((n-2k+1)⋯(n) ) * (n+1)^(n-2k-1) *
 *            C(n-k-1,k) where (n-2k+1)⋯(n) is the falling factorial of length
 *            2k. The implementation evaluates the sum iteratively, computing
 *            powers and binomial coefficients as needed. a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;

    /* initial term k = 0 */
    seq_t pow = big_pow((seq_t)(n + 1), (seq_t)(n - 1));   /* (n+1)^(n-1) */
    seq_t pref = 1;                                        /* product of zero terms */
    seq_t sum  = pow;                                       /* pref * pow * binom(…,0) */

    /* iterate over k = 1 .. ⌊n/2⌋ */
    for (idx_t k = 1; ; ++k)
    {
        /* stop if exponent n-2k-1 becomes negative (binomial would be 0) */
        if (n < 2 * k + 1) break;

        /* update pref: multiply by (n-2(k-1)) and (n-2(k-1)-1) */
        pref *= (seq_t)(n - 2 * (k - 1));
        pref *= (seq_t)(n - 2 * (k - 1) - 1);

        /* update pow: divide by (n+1)^2 */
        seq_t denom_pow = (seq_t)(n + 1) * (seq_t)(n + 1);
        pow /= denom_pow;

        /* binomial coefficient C(n-k-1, k) */
        seq_t binomk = binomial(n - k - 1, k);

        /* add term to sum */
        seq_t term = (pref * pow) * binomk;
        sum += term;
    }

    return sum;
}
