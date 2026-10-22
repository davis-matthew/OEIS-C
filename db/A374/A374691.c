/*
 * A374691  The smallest m+k such that n can be written as n=binomial(m,k).
 * https://oeis.org/A374691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   binom_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A374691: smallest m + k such that binomial(m,k) = n.
   We search over k, for each k find the minimal m with binomial(m,k) ≥ n
   using binary search on m.  If the binomial equals n we update the
   minimal sum.  The bound for k is 2*k ≤ current best sum (k=1 gives
   the trivial solution m=n, k=1 → sum = n+1).  The function works for
   any idx_t value of n (≥1). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom_limit.h"

/* external linkage */
seq_t a(idx_t n)
{
    if (n == 1) return 0;              /* trivial case m=k=0 */

    seq_t best = (seq_t)n + 1;         /* k = 1, m = n gives sum n+1 */

    /* iterate over k starting at 2, stop when 2*k > current best */
    for (idx_t k = 2; 2 * k <= (idx_t)best; ++k) {
        /* find minimal m >= k such that binom(m,k) >= n */
        idx_t low = k;
        idx_t high = k;
        seq_t val = binom_limit(high, k, (seq_t)n);
        while (val < n) {
            high <<= 1;                 /* double high */
            if (high > n) high = n;     /* guard against excessive growth */
            val = binom_limit(high, k, (seq_t)n);
            if (high == n && val < n) break;  /* cannot reach n */
        }
        if (high == n && val < n) continue;  /* no solution for this k */

        /* binary search for minimal m with binom(m,k) >= n */
        while (low < high) {
            idx_t mid = low + ((high - low) >> 1);
            seq_t midVal = binom_limit(mid, k, (seq_t)n);
            if (midVal >= n)
                high = mid;
            else
                low = mid + 1;
        }
        seq_t cand = binom_limit(low, k, (seq_t)n);
        if (cand == n) {
            seq_t s = low + k;
            if (s < best) best = s;
        }
    }

    return best;
}
