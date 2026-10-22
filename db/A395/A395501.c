/*
 * A395501  Number of ordered quadruples (w, x, y, z) of distinct positive
 *          integers such that w*x + y*z = n.
 * https://oeis.org/A395501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered quadruples of distinct positive
 *            integers (w, x, y, z) such that w*x + y*z = n. Computation:
 *            brute‑force iterate over all ordered (w, x) with w != x and w*x
 *            <= n-2. For each pair, let s = n - w*x. Count all ordered pairs
 *            (y, z) of distinct positive integers with y*z = s th...
 * Range:     u8 n <= 36, u16 n <= 1139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;

    seq_t res = 0;
    idx_t limit = n - 2;          /* maximum value for w*x to keep s >= 2 */

    for (idx_t w = 1; w < n; ++w) {
        for (idx_t x = 1; x < n; ++x) {
            if (w == x) continue;

            /* Check whether w*x would exceed limit without overflow. */
            if (w > limit / x) break;   /* as x increases, w*x increases */

            idx_t t = w * x;      /* safe: w <= limit/x */
            idx_t s = n - t;      /* s >= 2 by construction */

            /* Enumerate divisor pairs of s. */
            for (idx_t d = 1; d <= s / d; ++d) {
                if (s % d) continue;
                idx_t e = s / d;
                if (d == e) continue;   /* need distinct y, z */

                /* (d, e) as (y, z) */
                if (d != w && d != x && e != w && e != x) {
                    ++res;
                }
                /* (e, d) as (y, z) */
                if (e != w && e != x && d != w && d != x) {
                    ++res;
                }
            }
        }
    }

    return res;
}
