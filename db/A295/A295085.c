/*
 * A295085  Numbers k such that {k*phi} < 0.25 or {k*phi} > 0.75, where phi is
 *          the golden ratio (1 + sqrt(5))/2 and { } denotes fractional part.
 * https://oeis.org/A295085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (n+1)-st positive integer k such that * {k*phi} <
 *            0.25 or {k*phi} > 0.75, * where phi = (1+sqrt(5))/2 and {|x|}
 *            denotes the * fractional part of x. (All indices start at 0.)
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Find the n-th term (0‑based).  This routine performs a simple
 * search without caching; it is O(n) and easily fast enough for n ≤ 10⁴. */
seq_t a(idx_t n)
{
    /* We need the (n+1)-th valid k.  Count them until we reach it. */
    idx_t need = n + 1;
    idx_t count = 0;
    idx_t k = 1;

    /* Golden ratio as a long double constant.  (This use of floating
     * point is independent of the seq_t type.) */
    const long double phi = (1.0L + sqrtl(5.0L)) * 0.5L;

    /* Search until we find the required term. */
    for (;; ++k) {
        long double x   = k * phi;
        long double frac = x - floorl(x);   /* fractional part in [0,1) */

        if (frac < 0.25L || frac > 0.75L) {
            ++count;
            if (count == need)
                return (seq_t)k;
        }
    }
}
