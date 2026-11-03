/*
 * A399031  Least positive k such that some nontrivial proper divisor d of n
 *          satisfies d+k | n+k; 0 if no such k exists.
 * https://oeis.org/A399031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that some nontrivial proper divisor
 *            d of n satisfies d + k | n + k, 0 if no such k exists.
 *            Implementation follows the equivalent formulation: a(n) =
 *            min_{d|n,1<d<n} (lambda_d(n-d) - d) where lambda_d(m) is the
 *            least divisor of m greater than d
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t min_divisor_gt(idx_t m, idx_t d)
{
    /* assumes m > d */
    idx_t best = m;               /* m itself is always a divisor > d, if m > d */
    for (idx_t j = 2; j <= m / j; ++j)
    {
        if (m % j == 0)
        {
            if (j > d && j < best) best = j;
            idx_t k = m / j;
            if (k > d && k < best) best = k;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n <= 3) return 0;              /* primes and 1,2,3 have no nontrivial proper divisor */

    idx_t best_k = (idx_t)-1;          /* sentinel for "no value found yet" */

    /* enumerate all proper divisors 1<d<n */
    for (idx_t i = 2; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            /* process divisor d1 */
            idx_t m = n - d1;
            if (m > d1)                       /* need m > d to have a divisor > d */
            {
                idx_t lambda = min_divisor_gt(m, d1);
                idx_t k = lambda - d1;
                if (k < best_k) best_k = k;
                if (best_k == 1) return 1;   /* cannot find smaller than 1 */
            }

            idx_t d2 = n / i;
            if (d2 != d1)                   /* avoid duplication when d1 == d2 */
            {
                m = n - d2;
                if (m > d2)
                {
                    idx_t lambda = min_divisor_gt(m, d2);
                    idx_t k = lambda - d2;
                    if (k < best_k) best_k = k;
                    if (best_k == 1) return 1;
                }
            }
        }
    }

    if (best_k == (idx_t)-1) return 0;  /* no suitable divisor found */
    return (seq_t)best_k;
}
