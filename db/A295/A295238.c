/*
 * A295238  Expansion of e.g.f. 2/(1 + sqrt(1 - 4*x*exp(x))).
 * https://oeis.org/A295238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * Catalan(k) / (n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t ans = 0;
    seq_t falling = 1;   /* for k = 0 : (n!)/(n-0)! = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update falling factorial: multiply by (n-k+1) */
        falling *= (seq_t)(n - k + 1);

        /* k^(n-k) using big integer exponentiation */
        seq_t kpow = big_pow((seq_t)k, (seq_t)(n - k));

        /* Catalan(k) */
        seq_t cat = catalan(k);

        /* term = falling * cat * kpow */
        seq_t term = falling * cat * kpow;

        ans += term;
    }

    return ans;
}
