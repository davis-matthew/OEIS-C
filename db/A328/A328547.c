/*
 * A328547  Number of 3-regular bipartitions of n.
 * https://oeis.org/A328547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3‑regular bipartitions of n. This is obtained
 *            from the generating function ∏_{k≥1} (1 + 2x^k + 3x^{2k} +
 *            2x^{3k} + x^{4k}). The coefficient of x^n in that product is the
 *            desired value. The values are computed by dynamic programming
 *            without using arbitrary‑precision rational arith...
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate two arrays of seq_t of length n+1. */
    seq_t *r = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *s = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!r || !s) return (seq_t)0;   /* allocation failure – unlikely in tests */

    /* Initialize: r[0] = 1, all other entries 0. */
    for (idx_t i = 0; i <= n; ++i) r[i] = (seq_t)0;
    r[0] = (seq_t)1;

    /* DP: for each part size k, multiply the current polynomial
       by (1 + 2x^k + 3x^{2k} + 2x^{3k} + x^{4k}). */
    for (idx_t k = 1; k <= n; ++k) {
        /* Zero temporary array s. */
        for (idx_t i = 0; i <= n; ++i) s[i] = (seq_t)0;

        for (idx_t i = 0; i <= n; ++i) {
            seq_t val = r[i];
            if (!val) continue;               /* skip if zero */

            /* Offset 0, coefficient 1. */
            s[i] += val;

            /* Offset k, coefficient 2. */
            if (i + k <= n) s[i + k] += val * (seq_t)2;

            /* Offset 2k, coefficient 3. */
            if (i + 2 * k <= n) s[i + 2 * k] += val * (seq_t)3;

            /* Offset 3k, coefficient 2. */
            if (i + 3 * k <= n) s[i + 3 * k] += val * (seq_t)2;

            /* Offset 4k, coefficient 1. */
            if (i + 4 * k <= n) s[i + 4 * k] += val;
        }

        /* Swap r and s for the next iteration. */
        seq_t *tmp = r;
        r = s;
        s = tmp;
    }

    seq_t ans = r[n];
    free(r);
    free(s);
    return ans;
}
