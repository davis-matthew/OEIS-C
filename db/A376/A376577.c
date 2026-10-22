/*
 * A376577  E.g.f. A(x) satisfies A(x) = exp(x*(1 + x^2)*A(x)).
 * https://oeis.org/A376577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n-2*k+1)^(n-2*k-1) * *
 *            binomial(n-2*k,k)/(n-2*k)! (OEIS notation)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;      /* a(0) = 1 */

    idx_t kmax = n / 3;
    seq_t result = 0;
    seq_t prod = 1;          /* prod = n! / ( (n-2*k)! ) for current k */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 2 * k;                         /* m = n-2k */
        seq_t bin = binomial(m, k);                  /* binomial(m,k) */
        seq_t p   = pow_seq((seq_t)(m + 1), (int)(m - 1));   /* (m+1)^(m-1) */
        seq_t term = bin * p * prod;                 /* term for this k */
        result += term;

        /* Prepare prod for the next k (if any) */
        if (k < kmax) {
            prod *= (seq_t)m;
            prod *= (seq_t)(m - 1);
        }
    }

    return result;
}
