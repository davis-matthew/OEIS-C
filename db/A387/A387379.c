/*
 * A387379  a(n) = number of quartets (n,u,v,w) of type 4; see Comments.
 * https://oeis.org/A387379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{v=2}^{n−1} max(0, * ⌊(n−1)/(v/gcd(n,v))⌋ −
 *            ⌊v/(n/gcd(n,v))⌋ )
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n < 2) {
        return 0;
    }

    for (idx_t v = 2; v < n; ++v) {
        /* reduce with gcd to speed up the divisions   */
        idx_t d      = (idx_t)gcd((seq_t)n, (seq_t)v);
        idx_t n_div  = n / d;        /* n' = n / gcd(n,v)   */
        idx_t v_div  = v / d;        /* v' = v / gcd(n,v)   */

        /* y = n'*k must satisfy v < y <= n*(n-1)/v   */
        idx_t k_max = (n - 1) / v_div;           /* floor((n-1)/v')   */
        idx_t k_min = v / n_div + 1;             /* floor(v/n') + 1   */

        if (k_max >= k_min) {
            total += (seq_t)(k_max - k_min + 1);
        }
    }

    return total;
}
