/*
 * A379088  G.f. A(x) satisfies A(x) = (1 + x*A(x)^3) * (1 + x^2*A(x)^7).
 * https://oeis.org/A379088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(3*n+k+1, k) * binomial(3*n+k+1,
 *            n-2*k) / (3*n+k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements the above sum.  It uses a gcd
   reduction to avoid overflow of the intermediate product
   binomial(3*n+k+1,k) * binomial(3*n+k+1,n-2*k) before the
   division by (3*n+k+1).  This guarantees correctness for the
   64‑bit build as long as the final a(n) fits in seq_t, and works
   for arbitrary‑precision builds as well. */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n >> 1;                    /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* Common denominator d = 3*n + k + 1 */
        seq_t d = (seq_t)(3*n + k + 1);

        seq_t t1 = binom((idx_t)(3*n + k + 1), k);
        seq_t t2 = binom((idx_t)(3*n + k + 1), n - 2*k);

        /* Reduce t1 and t2 by gcd with the denominator to avoid overflow */
        seq_t g = gcd(t1, d);
        t1 /= g;
        d /= g;

        g = gcd(t2, d);
        t2 /= g;
        d /= g;   /* d should now be 1 */

        seq_t term = t1 * t2;   /* exact division already performed */
        result += term;
    }

    return result;
}
