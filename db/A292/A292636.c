/*
 * A292636  Rank of (3-r)*n when all the numbers (3-r)*j and (3+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor( n * (3 - sqrt(2)) / (3 + sqrt(2)) )
 * Range:     u8 n <= 7, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   f_nonpositive, g_positive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (computed using integer inequalities without floating‑point arithmetic) */

#include <stddef.h>
#include "../utils/f_nonpositive.h"
#include "../utils/g_positive.h"

/* Auxiliary predicates implemented with integer arithmetic.
   They test the inequalities derived from the Beatty‑sequence definition.
   For n, k >= 0: g(k) > 0  ⇔  2*(n + k + 1)^2  >  9*(n - k - 1)^2
   f(k) <= 0  ⇔  2*(k + n)^2   <=  9*(n - k)^2
   These formulas avoid any reference to sqrt(2).                        */

seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term.  For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Binary search to find the smallest k ≥ 0 such that g(k) > 0.
       Over the interval [0, n] the function g(k) is monotonically
       increasing (the quadratic changes sign only after k > 1.57 n),
       so a classic binary search works. */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (g_positive((seq_t)n, (seq_t)mid))
            hi = mid;
        else
            lo = mid + 1;
    }
    seq_t k = lo;

    /* ..but for safety verify the complementary inequality f(k) ≤ 0.
       If it fails (which should not happen for k in the expected range),
       adjust k downward until it holds. */
    while (!f_nonpositive((seq_t)n, k)) {
        k--;
    }

    /* The rank is a(n) = n + k. */
    return (seq_t)n + k;
}
