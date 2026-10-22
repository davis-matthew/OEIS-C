/*
 * A363354  E.g.f. satisfies A(x) = exp(x * (1 + x * A(x)^3)).
 * https://oeis.org/A363354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*n-3*k+1)^(k-1) * binomial(k, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t fac_ratio = 1;          /* product_{i=k+1..n} i, updated while k decreases */

    for (idx_t k = n; k > 0; --k) {
        /* Update factorial ratio for the new k */
        if (k < n)
            fac_ratio = fac_ratio * (seq_t)(k + 1);

        idx_t j = n - k;                         /* j = n - k */
        seq_t base = (seq_t)(3 * j + 1);           /* (3*j + 1) */
        seq_t power = seq_pow(base, k - 1);       /* (3*j + 1)^(k-1) */
        seq_t bin    = binomial(k, j);            /* binomial(k, n-k) */

        seq_t term = fac_ratio * power;          /* (n!/k!) * base^(k-1) */
        term = term * bin;                       /* * binomial(k, n-k)   */

        sum = sum + term;
    }

    return sum;
}
