/*
 * A349849  Number of transitive relations on an n-set with exactly four
 *          ordered pairs.
 * https://oeis.org/A349849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,2) + 42*C(n,3) + 375*C(n,4) + 1450*C(n,5) +
 *            2940*C(n,6) + 3360*C(n,7) + 1680*C(n,8) (OEIS A349849) where
 *            C(n,k) = binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 25, u64 n <= 382,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: compute binomial(n,k) for 0 ≤ k ≤ n, using only integer arithmetic.
   It returns 0 for k > n.  The computation uses the multiplicative formula
   and divides by i at each step to keep intermediate values small. */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - i + 1) / i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* C(n,2)=0 for n<2 */

    seq_t ans = 0;
    ans += binom_small(n, 2);                     /* coefficient 1   */
    ans += 42 * binom_small(n, 3);                /* coefficient 42 */
    ans += 375 * binom_small(n, 4);               /* coefficient 375 */
    ans += 1450 * binom_small(n, 5);              /* coefficient 1450 */
    ans += 2940 * binom_small(n, 6);              /* coefficient 2940 */
    ans += 3360 * binom_small(n, 7);              /* coefficient 3360 */
    ans += 1680 * binom_small(n, 8);              /* coefficient 1680 */
    return ans;
}
