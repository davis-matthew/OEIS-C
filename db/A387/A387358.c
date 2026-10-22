/*
 * A387358  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*k,n-k).
 * https://oeis.org/A387358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0}^n \binom{n}{k} \binom{3k}{\,n-k\,}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Compute A387358(a(n)) for n >= 0.  Does not use static data and
   works for any seq_t and idx_t provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    seq_t binom_n = 1;   /* C(n,0) */
    seq_t M = 0;         /* C(3k, n-k) for current k */

    /* first k for which C(3k, n-k) can be non‑zero  */
    idx_t k0 = (n + 3) / 4;   /* ceil(n/4) */

    for (idx_t k = 0; k <= n; ++k) {
        /* add current term when it can be non‑zero */
        if (k >= k0) {
            seq_t term = binom_n * M;
            sum += term;
        }
        if (k == n)  break;   /* no further update needed */

        /* update binom_n for the next index k+1 */
        binom_n = binom_n * (seq_t)(n - k) / (seq_t)(k + 1);

        /* update M = C(3(k+1), n-(k+1)) */
        if (k + 1 == k0) {
            M = binomial(3 * (k + 1), n - (k + 1));
        } else if (k + 1 > k0) {
            /* ratio for successive k values */
            seq_t num = M;
            num *= (seq_t)(3 * k + 1);
            num *= (seq_t)(3 * k + 2);
            num *= (seq_t)(3 * k + 3);
            num *= (seq_t)(n - k);

            seq_t denom = 1;
            denom *= (seq_t)(4 * k - n + 1);
            denom *= (seq_t)(4 * k - n + 2);
            denom *= (seq_t)(4 * k - n + 3);
            denom *= (seq_t)(4 * k - n + 4);

            M = num / denom;
        }
    }

    return sum;
}
