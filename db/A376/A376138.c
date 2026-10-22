/*
 * A376138  a(n) is the smallest k such that n = ab + cd with 1 <= a,b,c,d <=
 *          k.
 * https://oeis.org/A376138
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{i=1..n/2} max(A033677(i), A033677(n-i)) where
 *            A033677(k) is the smallest t such that k = u*v with 1 <= u,v <=
 *            t. A033677(k) is implemented as k divided by its largest divisor
 *            <= sqrt(k)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t A033677(seq_t x) {
    if (x <= 1) return 1;            /* 1 = 1*1 */
    seq_t root = floor_sqrt(x);      /* floor(sqrt(x)) */
    idx_t d;
    for (d = (idx_t)root; d > 0; --d) {
        if (x % d == 0) break;       /* first (largest) divisor <= sqrt */
    }
    return x / (seq_t)d;             /* t = k / d */
}

seq_t a(idx_t n) {
    if (n < 2) return 0;             /* undefined for n < 2, return 0 */

    /* Allocate temporary array to store A033677 values up to n */
    seq_t *val = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!val) return 0;             /* allocation failure */

    for (idx_t i = 1; i <= n; ++i) {
        val[i] = A033677((seq_t)i);
    }

    /* Find the minimum over i of max(val[i], val[n-i]) */
    seq_t best = (seq_t)n;           /* upper bound for a(n) */
    for (idx_t i = 1; i <= n / 2; ++i) {
        seq_t m = val[i] > val[n - i] ? val[i] : val[n - i];
        if (m < best) best = m;
    }

    free(val);
    return best;
}
