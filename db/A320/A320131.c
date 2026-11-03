/*
 * A320131  Number of integer solutions to a^2 + b^2 + 2*c^2 + 9*d^2 = n.
 * https://oeis.org/A320131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + b^2 + 2*c^2 + 9*d^2
 *            = n Smoothed by counting signs separately
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9051 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

static seq_t _s_r(idx_t n)
{
    /* Count integer pairs (a,b) with a^2 + b^2 = n.
       Handles sign variations: (0,0) -> 1, one zero -> 2, both non‑zero -> 4. */
    seq_t cnt = 0;
    /* integer sqrt of n */
    idx_t a_max = 0;
    while ((a_max + 1) * (a_max + 1) <= n) ++a_max;
    for (idx_t a = 0; a <= a_max; ++a) {
        idx_t a2 = a * a;
        idx_t b2 = n - a2;
        if (b2 > (idx_t)(-1)) continue; /* guard, though unlikely for n <= 10^5 */
        /* integer sqrt of b2 */
        idx_t b = 0;
        while ((b + 1) * (b + 1) <= b2) ++b;
        if (b * b != b2) continue;
        /* count sign variations */
        if (a == 0 && b == 0) {
            cnt += 1;
        } else if (a == 0 || b == 0) {
            cnt += 2;
        } else {
            cnt += 4;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* iterate over non‑negative d; multiply by 2 for d>0 to account for ±d */
    for (idx_t d = 0; d * d * 9 <= n; ++d) {
        idx_t d2 = d * d * 9;
        idx_t rem_d = n - d2;
        seq_t multD = (d == 0) ? 1 : 2;
        /* iterate over non‑negative c; multiply by 2 for c>0 */
        for (idx_t c = 0; c * c * 2 <= rem_d; ++c) {
            idx_t c2 = c * c * 2;
            idx_t rem_c = rem_d - c2;
            seq_t multC = (c == 0) ? 1 : 2;
            seq_t abCount = _s_r(rem_c);
            total += multD * multC * abCount;
        }
    }
    return total;
}
