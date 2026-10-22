/*
 * A315050  Coordination sequence Gal.5.140.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A315050: Coordination sequence Gal.5.140.2
 * Recurrence: a(0)=1.  For n≥1
 *   a(n)=a(n‑1)+d(n)
 *   where
 *     d(1..9)  = {4,4,6,5,4,5,6,4,5}
 *     d(n≥10) = {5,4,6,5,4,5,6,4,5}[(n-10) mod 9]
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    static const unsigned char init[9] = {4,4,6,5,4,5,6,4,5};
    static const unsigned char cycle[9] = {5,4,6,5,4,5,6,4,5};

    seq_t val = 1;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned d;
        if (i <= 9)          d = init[i-1];
        else                 d = cycle[(i-10) % 9];
        val += (seq_t)d;
    }
    return val;
}
