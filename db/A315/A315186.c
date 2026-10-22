/*
 * A315186  Coordination sequence Gal.6.263.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(n)=a(n-1)+g((n-2) mod 11) for n≥2, where
 *            g=[4,4,4,6,4,6,4,4,4,6,6]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t g[11] = {4,4,4,6,4,6,4,4,4,6,6};
static const seq_t pref[12] = {
    0, 4, 8, 12, 18, 22, 28, 32, 36, 40, 46, 52
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    idx_t m = n - 1;                     /* number of differences after a(1) */
    idx_t full = m / 11;                  /* complete periods of 11 */
    idx_t rem  = m % 11;                  /* remainder */

    seq_t sum = (seq_t)full * (seq_t)52;   /* 52 = sum of one full period */
    sum += pref[rem];                     /* add partial period */

    return (seq_t)6 + sum;                /* a(1) + accumulated differences */
}
