/*
 * A363683  Square array A(n, k), n, k > 0, read by antidiagonals; A(n, k) is
 *          the least e > 0 such that n^e and k^e have the same initial digit,
 *          or -1 if no such e exists.
 * https://oeis.org/A363683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A(n, k) is the least e > 0 such that the leading
 * decimal digits of n^e and k^e coincide.  The sequence a(n) is
 * the array read by antidiagonals.
 *
 * This implementation maps n to the pair (i, j), then iterates
 * increasing e computing the leading digit of i^e and j^e by
 * high‑precision arithmetic on long double.  No arbitrary‑precision
 * integer powers are built; only the leading digit is required.
 *
 * The algorithm runs in O(e) time per call.  For the first 10 000
 * terms e never exceeds about a few hundred, so the routine runs in
 * well under the 2 s limit.
 */

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/_floor_isqrt_u64.h"

static inline uint8_t leading_digit(idx_t base, long double log_base, idx_t e)
{
    long double f = e * log_base;
    long double frac = f - floorl(f);
    long double ld = powl(10.0L, frac);
    /* Avoid rounding to 10 */
    if (ld >= 10.0L) ld = 9.999999999999999L;
    return (uint8_t)(ld + 1e-12L);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;        /* undefined offset, but harmless */
    if (n == 1) return (seq_t)1;         /* A(1,1) = 1 */

    /* ---- map n to coordinates (i, j) on the anti‑diagonal ---- */
    idx_t d;
    {
        idx_t t = _floor_isqrt_u64(1 + 8 * n);
        d = (t - 1) / 2;
        while (d * (d + 1) / 2 < n) ++d;
    }
    idx_t prev_tri = d * (d - 1) / 2;
    idx_t k = n - prev_tri;          /* kth entry on diagonal d (1‑based) */
    idx_t i = k;
    idx_t j = d - k + 1;

    if (i == j) return (seq_t)1;     /* diagonal entries are 1 */

    long double log_i = log10l((long double)i);
    long double log_j = log10l((long double)j);

    idx_t e = 1;
    for (;; ++e) {
        if (leading_digit(i, log_i, e) == leading_digit(j, log_j, e))
            return (seq_t)e;
    }
}
