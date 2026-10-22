/*
 * A332652  a(n) = Sum_{k=1..n} n^(k/gcd(n, k)).
 * https://oeis.org/A332652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} n^(k/gcd(n, k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd_idx.h"

/* Euclidean algorithm for idx_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Pre‑compute powers n^1, n^2, …, n^n */
    seq_t *pow = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow) return 0;           /* out‑of‑memory case */
    pow[0] = 1;                   /* unused but convenient */
    seq_t base = (seq_t)n;
    pow[1] = base;
    for (idx_t i = 2; i <= n; ++i) {
        pow[i] = pow[i - 1] * base;   /* works for native ints and
                                         bespoke big‑int classes   */
    }

    /* Sum over k */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t g   = gcd_idx(n, k);
        idx_t exp = k / g;            /* exponent = k/gcd(n,k)          */
        sum += pow[exp];
    }

    free(pow);
    return sum;
}
