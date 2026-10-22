/*
 * A377608  E.g.f. satisfies A(x) = exp( x * A(x) / (1-x) ) / (1-x)^2.
 * https://oeis.org/A377608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (n! / k!) * (k+1)^(k-1) * binomial(n+2k+1,
 *            n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utilities: big_pow, binomial.                                  */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t f = 1;                     /* f holds n! / k! for current k; start with k = n */

    for (idx_t k = n; ; --k)
    {
        seq_t pk;
        if (k == 0) {
            /* (k+1)^(k-1) with k=0 is defined as 1 */
            pk = 1;
        } else {
            pk = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
        }

        seq_t bin = binomial((idx_t)(n + 2 * k + 1), (idx_t)(n - k));

        seq_t term = f * pk;
        term = term * bin;
        sum += term;

        if (k == 0) break;           /* finished loop */
        f = f * ( (seq_t)k );        /* update f for next k-1 : f_{k-1} = f_k * k */
    }

    return sum;
}
