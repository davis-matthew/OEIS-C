/*
 * A311651  Coordination sequence Gal.3.22.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-12) + 56 for n >= 4, with the initial values a(0)=1,
 *            a(1)=4, a(2)=8, a(3)=12 and a(4..15) =
 *            20,20,32,32,36,40,52,48,56,60,68,68
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


static const seq_t _init0[4] = {
    (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)12
};

static const seq_t _period[12] = {
    (seq_t)20, (seq_t)20, (seq_t)32, (seq_t)32,
    (seq_t)36, (seq_t)40, (seq_t)52, (seq_t)48,
    (seq_t)56, (seq_t)60, (seq_t)68, (seq_t)68
};

seq_t a(idx_t n)
{
    if (n < 4) {
        return _init0[(size_t)n];
    }
    idx_t k = (n - 4) / 12;          /* number of full 12‑step periods after 4 */
    idx_t r = (n - 4) % 12;          /* position within the period */
    return _period[(size_t)r] + (seq_t)56 * (seq_t)k;
}
