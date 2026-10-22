/*
 * A365157  G.f. satisfies A(x) = ( 1 + x*A(x)^2*(1 + x*A(x))^3 )^2.
 * https://oeis.org/A365157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n}
 *            binomial(2*(n+k+1),k)*binomial(3*k,n-k)/(n+k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the recurrence A(x) = (1 + x*A(x)^2*(1 + x*A(x))^3)^2 (s=3, t=2) */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute the two binomial factors */
        seq_t bin1 = binomial((idx_t)(2 * (n + k + 1)), (idx_t)k);
        seq_t bin2 = binomial((idx_t)(3 * k), (idx_t)(n - k));

        /* skip terms that are zero */
        if (!bin1 || !bin2)
            continue;

        /* reduce the product by the common divisor (n+k+1) to stay within 64‑bit
           when seq_t is uint64_t; works for arbitrary precision as well */
        seq_t divisor = (seq_t)(n + k + 1);
        seq_t g = gcd(bin1, divisor);
        bin1 /= g;
        divisor /= g;

        g = gcd(bin2, divisor);
        bin2 /= g;
        divisor /= g;  /* divisor should now be 1 */

        /* multiply the reduced terms and add to the sum */
        seq_t term = bin1 * bin2;
        sum += term;
    }
    return sum;
}
