/*
 * A334526  Number of triples (w,x,y) with all terms in {-n,...,0,...,n} and
 *          w^2 + 11xy = 0.
 * https://oeis.org/A334526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 4*n + 1 + 4 * sum_{t=1}^{⌊n/11⌋} C_t where C_t = number
 *            of ordered pairs (a,b) with a*b = 11*t^2 and 1 <= a <= n, 1 <= b
 *            <= n. This counts triples (w,x,y) with w^2 + 11*x*y = 0 and
 *            w,x,y ∈ [−n,n]
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    // Base contribution from pairs with x*y = 0 (w must be 0)
    seq_t ans = ((seq_t)4 * (seq_t)n) + 1;

    idx_t tmax = n / 11;   // largest t with 11*t <= n
    for (idx_t t = 1 ; t <= tmax ; ++t) {
        idx_t k = 11 * t * t;   // k = 11*t^2
        idx_t count = 0;        // ordered pairs (a,b) with a*b=k, a,b <= n

        // Iterate over divisors d <= sqrt(k)
        for (idx_t d = 1 ; d * d <= k ; ++d) {
            if (k % d == 0) {
                idx_t e = k / d;
                if (d <= n && e <= n) {
                    if (d == e)
                        count += 1;   // pair (d,d) counts once
                    else
                        count += 2;   // pairs (d,e) and (e,d)
                }
            }
        }
        // Each such pair yields 4 triples (±w, with two sign assignments for x or y)
        ans += (seq_t)4 * (seq_t)count;
    }
    return ans;
}
