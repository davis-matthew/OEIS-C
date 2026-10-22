/*
 * A363357  E.g.f. satisfies A(x) = exp(x * A(x)^2 * (1 + x * A(x))).
 * https://oeis.org/A363357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+k+1)^(k-1) * binomial(k,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                       /* a(0) = 1 */

    /* Only k >= ceil(n/2) contribute because binomial(k, n-k) = 0 otherwise. */
    idx_t ceil_half = (n + 1) / 2;

    seq_t sum   = 0;
    seq_t ratio = 1;           /* ratio = n! / k! for current k, starting with k = n */

    for (idx_t k = n; k >= ceil_half; --k) {
        seq_t power = pow_seq((seq_t)(n + k + 1), (int)(k - 1));          /* (n+k+1)^(k-1) */
        seq_t bin   = binomial(k, n - k);                                 /* binomial(k, n-k) */

        seq_t term = ratio;
        term *= power;
        term *= bin;

        sum += term;

        if (k > ceil_half)        /* prepare ratio for next smaller k (k-1) */
            ratio *= (seq_t)k;
    }

    return sum;
}
