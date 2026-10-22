/*
 * A382086  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * C(x))
 *          ), where C(x) = 1 + x*C(x)^2 is the g.f. of A000108.
 * https://oeis.org/A382086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{k=0..n-1} (n+1)^(n-k-1) *
 *            binomial(n+k-1,k)/(n-k-1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            (explicit summation derived from the e.g.f. definition) */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t sum = 0;

    /* v = (n+1)^(n-1)  (k = 0 term) */
    seq_t v = big_pow((seq_t)(n + 1), (seq_t)(n - 1));

    for (idx_t k = 0; k < n; ++k)
    {
        sum += v;

        if (k + 1 == n) break;   /* last term processed */

        idx_t knext = k + 1;

        seq_t num1 = (seq_t)(n - k - 1);   /* n-k-1   */
        seq_t num2 = (seq_t)(n + k);       /* n+k     */
        seq_t den1 = (seq_t)knext;         /* k+1     */
        seq_t den2 = (seq_t)(n + 1);       /* n+1     */

        seq_t tmp = v;

        /* cancel denominator with current value */
        seq_t g = gcd(tmp, den1);
        tmp /= g;  den1 /= g;

        g = gcd(tmp, den2);
        tmp /= g;  den2 /= g;

        /* multiply by numerator parts */
        tmp *= num1;
        tmp *= num2;

        /* cancel any remaining denominator factors */
        g = gcd(tmp, den1);
        tmp /= g;  den1 /= g;

        g = gcd(tmp, den2);
        tmp /= g;  den2 /= g;

        if (den1 != (seq_t)1) tmp /= den1;
        if (den2 != (seq_t)1) tmp /= den2;

        v = tmp;
    }

    return sum;
}
